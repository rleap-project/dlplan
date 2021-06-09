#ifndef DLP_SRC_CORE_ELEMENTS_ELEMENT_H_
#define DLP_SRC_CORE_ELEMENTS_ELEMENT_H_

#include "types.h"
#include "../state.h"
#include "../instance_info.h"
#include "../vocabulary_info.h"


#include <iostream>


namespace dlp {
namespace core {
namespace element {

template<typename T>
class Element {
protected:
    // The name.
    const std::string m_name;
    // Evaluation result
    T m_result;
    const State* m_state;

protected:
    virtual const T& evaluate_impl(const State& state) = 0;

public:
    Element(const VocabularyInfo&, const std::string& name) : m_name(name), m_result(T()), m_state(nullptr) { }
    virtual ~Element() = default;

    /**
     * Evaluate and cache the last result.
     */
    const T& evaluate(const State& state) {
        if (m_state != &state) {
            m_state = &state;
            m_result = evaluate_impl(state);
        }
        return m_result;
    }

    /**
     * Returns the complexity of the element.
     */
    virtual unsigned compute_complexity() const = 0;

    /**
     * Computes a representation that uniquely identifies the element.
     * TODO(dominik): Use more sophisticated equivalences.
     * TODO(dominik): Cache the result if this becomes a performance bottleneck (which probably won't).
     */
    virtual std::string compute_repr() const = 0;
};

}
}
}

#endif
