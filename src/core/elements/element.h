#ifndef DLPLAN_SRC_CORE_ELEMENTS_ELEMENT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ELEMENT_H_

#include "types.h"
#include "../state.h"
#include "../instance_info.h"
#include "../vocabulary_info.h"
#include "../cache.h"


#include <iostream>

namespace dlplan::core::element {


template<typename T>
class Element {
protected:
    // The name.
    const std::string m_name;

public:
    Element(const VocabularyInfo&, const std::string& name) : m_name(name) { }
    virtual ~Element() = default;

    /**
     * Evaluate and cache the last result.
     */
    virtual T evaluate(const State& state) const = 0;

    /**
     * Returns the complexity of the element.
     */
    virtual int compute_complexity() const = 0;

    /**
     * Computes a representation that uniquely identifies the element.
     * TODO(dominik): Use more sophisticated equivalences.
     * TODO(dominik): Cache the result if this becomes a performance bottleneck (which probably won't).
     */
    virtual std::string compute_repr() const = 0;
};

}

#endif
