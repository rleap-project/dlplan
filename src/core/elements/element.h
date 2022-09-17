#ifndef DLPLAN_SRC_CORE_ELEMENTS_ELEMENT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ELEMENT_H_

#include <iostream>
#include <sstream>
#include <mutex>
#include <cassert>

#include "types.h"

#include "../../../include/dlplan/utils/cache.h"
#include "../../../include/dlplan/core.h"
#include "../../../include/dlplan/utils/segmented_vector.h"

using namespace std::string_literals;

namespace dlplan::core::element {
class EvaluationCaches;

template<typename T>
class Element : public utils::Cachable {
protected:
    /**
     * Index can be used for internal caching.
     */
    int m_index;

public:
    // Elements are not copieable because they must live in the cache.
    explicit Element(const VocabularyInfo&) : m_index(-1) { }
    Element(const Element& other) = delete;
    Element& operator=(const Element& other) = delete;
    Element(Element&& other) = delete;
    Element& operator=(Element&& other) = delete;
    virtual ~Element() = default;

    virtual T evaluate(const State& state) const = 0;
    virtual T evaluate(const State& state, EvaluationCaches& cache) const = 0;

    virtual int compute_complexity() const = 0;

    virtual void compute_repr(std::stringstream& out) const = 0;

    void set_index(int index) {
        m_index = index;
    }

    virtual std::string compute_repr() const {
        std::stringstream ss;
        compute_repr(ss);
        return ss.str();
    }

    int get_index() const {
        return m_index;
    }
};

/**
 * Thread-safe cache for concept and role denotations per state.
 */
template<typename ELEMENT_TYPE, typename DENOTATION_TYPE>
class EvaluationCache {
private:
    utils::SegmentedVector<utils::SegmentedVector<DENOTATION_TYPE>> m_denotations;
    utils::SegmentedVector<utils::SegmentedVector<bool>> m_status;

    /**
     * For multi-threading purposes
     */
    mutable std::mutex m_status_lock;
    mutable std::mutex m_denotations_lock;

public:
    EvaluationCache(int num_objects);
    ~EvaluationCache();

    /**
     * Returns 1 if denotation is cached else 0.
     */
    int count(const State& state, const ELEMENT_TYPE& element) {
        std::lock_guard<std::mutex> hold(m_status_lock);
        return m_status[state.get_index()][element.get_index()];
    }

    /**
     * Returns the denotation from the cache.
     * Throws an exception if denotation is not present.
     */
    DENOTATION_TYPE find(const State& state, const ELEMENT_TYPE& element) {
        std::lock_guard<std::mutex> hold(m_denotations_lock);
        assert(count(state, element));
        return m_denotations[state.get_index()][element.get_index()];
    }

    /**
     * Inserts the denotation into the cache.
     *
     */
    void insert(const State& state, const ELEMENT_TYPE& element, const DENOTATION_TYPE& denotation) {
        std::lock_guard<std::mutex> hold(m_denotations_lock);
        assert(!count(state, element));
        m_denotations[state.get_index()][element.get_index()] = denotation;
    }
};

/**
 * Maybe we want to have a single cache for all individual concepts and roles.
 * Hence, to reduce the number of arguments and for extendability
 * we store all caches in a struct.
 */
struct EvaluationCaches {
    EvaluationCache<element::Concept, ConceptDenotation> m_concept_denotation_cache;
    EvaluationCache<element::Role, RoleDenotation> m_role_denotation_cache;
};

}

#endif
