#ifndef DLP_SRC_CORE_ELEMENTS_ELEMENT_H_
#define DLP_SRC_CORE_ELEMENTS_ELEMENT_H_

#include "types.h"
#include "../types.h"
#include "../cache.h"
#include "../state.h"


namespace dlp {
namespace core {
class InstanceInfoImpl;
namespace element {

template<typename T>
class Element {
protected:
    // Non-owning parent pointer
    const InstanceInfoImpl* m_parent;
    // Evaluation result
    T m_result;
    const StateImpl *m_state;
    // TODO(dominik): we might want cache goal evaluations of different instances

protected:
    virtual T evaluate_impl(const StateImpl& state) = 0;

public:
    Element(const InstanceInfoImpl* parent) : m_parent(parent) { }

    /**
     * Evaluate and cache the last result.
     */
    virtual T evaluate(const StateImpl& state) {
        if (m_parent != state.m_parent) {
            throw std::invalid_argument("RoleElement::evaluate - instance information between state and element do not match.");
        }
        if (m_state != &state) {
            m_state = &state;
            m_result = evaluate_impl(state);
        }
        return m_result;
    }

    /**
     * Returns the complexity of the element.
     */
    virtual unsigned complexity() const = 0;
};

}
}
}

#endif
