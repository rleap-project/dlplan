#ifndef DLPLAN_SRC_CORE_ELEMENTS_ELEMENT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ELEMENT_H_

#include <iostream>
#include <sstream>
#include <mutex>
#include <cassert>
#include <memory>

#include "types.h"

#include "../../../include/dlplan/utils/cache.h"
#include "../../../include/dlplan/core.h"


using namespace std::string_literals;

namespace dlplan::core::element {
class GeneratorEvaluationCaches;

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
    virtual const T* evaluate(const State& state, GeneratorEvaluationCaches& cache) const = 0;

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
 * In our feature generator, we can assume that no two threads
 * compute the same denotation and that no computation
 * depends on the computation of another denotation
 * because we compute elements with same complexity.
 */
template<typename DENOTATION_TYPE>
struct CacheEntry {
    DENOTATION_TYPE m_denotation;
    bool m_status;

    std::mutex m_mutex;

    CacheEntry(int num_objects)
        : m_denotation(DENOTATION_TYPE(num_objects)), m_status(false) { }

    /**
     * A CacheEntry is lock free readable if it is cached.
     * This function is useful for assertions.
     */
    bool is_lock_free_readable() const {
        return m_status;
    }
};

/**
 * A wrapper around std::vector that automatically resizes
 * upon access such that index fits into it.
 */
template<typename T, typename Alloc=std::allocator<T>>
class AutoResizeVector {
private:
    std::vector<T, Alloc> m_data;

public:
    T& operator[](int index) {
        if (index >= static_cast<int>(m_data.size())) {
            m_data.resize(index + 1);
        }
        return m_data[index];
    }

    auto begin() {
        return m_data.begin();
    }

    auto end() {
        return m_data.end();
    }
};

/**
 * Thread-safe cache for concept and role denotations per state.
 */
template<typename ELEMENT_TYPE, typename DENOTATION_TYPE>
class GeneratorEvaluationCache {
private:
    /**
     * After accessing the CacheEntry, we return it and unlock the cache.
     * Access to the CacheEntry after return will remain thread safe.
     *
     * For simplicity we used unordered_maps but we might want to use vector instead
     */
    AutoResizeVector<AutoResizeVector<std::unordered_map<int, CacheEntry<DENOTATION_TYPE>*>>> m_denotations;

    std::mutex m_mutex;

public:
    GeneratorEvaluationCache() { }
    ~GeneratorEvaluationCache() {
        std::lock_guard<std::mutex> hold(m_mutex);
        for (auto& nested_1 : m_denotations) {
            for (auto& nested_2 : nested_1) {
                for (auto& pair : nested_2) {
                    delete pair.second;
                }
            }
        }
    }

    /**
     * Deletes a previously generated cache entry.
     */
    void erase(const State& state, const ELEMENT_TYPE& element) {
        std::lock_guard<std::mutex> hold(m_mutex);
        assert(state.get_instance_info_ref().get_index() >= 0);
        assert(state.get_index() >= 0);
        assert(element.get_index() >= 0);
        auto& denotations = m_denotations[state.get_instance_info_ref().get_index()][state.get_index()];
        auto result = denotations.insert(std::make_pair(element.get_index(), nullptr));
        assert(result);
        delete result;
        denotations.erase(element.get_index());
    }

    /**
     * Returns a reference to the cache entry.
     * User must check
     */
    CacheEntry<DENOTATION_TYPE>* find(const State& state, const ELEMENT_TYPE& element) {
        std::lock_guard<std::mutex> hold(m_mutex);
        assert(state.get_instance_info_ref().get_index() >= 0);
        assert(state.get_index() >= 0);
        assert(element.get_index() >= 0);
        // Attempt to insert nullptr.
        auto result = m_denotations[state.get_instance_info_ref().get_index()][state.get_index()].insert(
            std::make_pair(element.get_index(), nullptr));
        if (result.second) {
            // nullptr was inserted, i.e., no denotation existed,
            // Hence, we can allocate and insert a new denotation.
            result.first->second = new CacheEntry<DENOTATION_TYPE>(state.get_instance_info_ref().get_num_objects());
        }
        return result.first->second;
    }
};

/**
 * Pass all caches to the element evaluation function as single argument.
 * If we cache boolean and numericals here, we can also get rid of the evaluator component.
 */
struct GeneratorEvaluationCaches {
    GeneratorEvaluationCache<element::Boolean, bool> m_boolean_denotation_cache;
    GeneratorEvaluationCache<element::Numerical, int> m_numerical_denotation_cache;
    GeneratorEvaluationCache<element::Concept, ConceptDenotation> m_concept_denotation_cache;
    GeneratorEvaluationCache<element::Role, RoleDenotation> m_role_denotation_cache;
};

}

#endif
