#ifndef DLPLAN_SRC_CORE_ELEMENTS_ELEMENT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ELEMENT_H_

#include "../../../include/dlplan/types.h"
#include "../state.h"
#include "../instance_info.h"
#include "../vocabulary_info.h"
#include "../per_element_evaluation_cache.h"

#include <iostream>
#include <sstream>


namespace dlplan::core::element {


template<typename T>
class Element {
protected:
    const std::string m_name;
    int m_index;

public:
    /**
     * Index is set after insertion to the cache.
     * Index=-1 indicates that the element is not stored in a cache.
     */
    Element(const VocabularyInfo&, const std::string& name) : m_name(name), m_index(-1) { }
    virtual ~Element() = default;

    virtual T evaluate(const State& state, PerElementEvaluationCache& caches) const = 0;

    virtual int compute_complexity() const = 0;

    virtual void compute_repr(std::stringstream& out) const = 0;

    std::string compute_repr() const {
        std::stringstream ss;
        compute_repr(ss);
        return ss.str();
    }

    void set_index(int index) {
        assert(index >= 0);
        m_index = index;
    }

    int get_index() const {
        return m_index;
    }
};

}

#endif
