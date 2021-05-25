#ifndef DLP_SRC_CORE_ELEMENTS_ELEMENT_H_
#define DLP_SRC_CORE_ELEMENTS_ELEMENT_H_

#include "types.h"
#include "../types.h"
#include "../cache.h"


namespace dlp {
namespace core {
namespace element {

template<typename T>
class Element {
protected:
    bool m_goal;
    T m_result;
    const Index_Vec *m_state_atoms;
    // TODO(dominik): we might want cache goal evaluations of different instances

protected:
    virtual T evaluate_impl(const Index_Vec& atoms) = 0;

public:
    Element(bool goal) : m_goal(goal) { }

    /**
     * Evaluate and cache the last result.
     */
    virtual T evaluate(const InstanceInfoImpl& info, const Index_Vec& state_atoms) {
        if (m_state_atoms != &state_atoms) {
            if (m_goal) {
                m_state_atoms = &info.goal_atom_idxs();
                m_result = evaluate_impl(info.goal_atom_idxs());
            } else {
                m_state_atoms = &state_atoms;
                m_result = evaluate_impl(state_atoms);
            }
        }
        return m_result;
    }

    /**
     * Returns the complexity of the element.
     */
    virtual unsigned complexity() const = 0;

    /**
     * Getters.
     */
    virtual bool goal() const {
        return m_goal;
    }
};

}
}
}

#endif
