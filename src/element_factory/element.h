#ifndef DLP_SRC_FEATURE_FACTORY_ELEMENT_H_
#define DLP_SRC_FEATURE_FACTORY_ELEMENT_H_

#include "types.h"

namespace dlp {


template<typename T>
class Element {
protected:
    T m_result;
    const Index_Vec *m_state_atoms;
    bool m_goal;

protected:

    virtual T evaluate_impl(const Index_Vec& atoms) = 0;

public:
    Element(bool goal, const Index_Vec& goal_atoms) : m_goal(goal) {
        if (goal) {
            m_result = evaluate_impl(goal_atoms);
        }
    }

    /**
     * Evaluate and cache the last result.
     */
    virtual T evaluate(const Index_Vec& state_atoms) {
        if (!m_goal && (m_state_atoms != &state_atoms)) {
            m_state_atoms = &state_atoms;
            m_result = evaluate_impl(state_atoms);
        }
        return m_result;
    }

    /**
     * The size of the result.
     */
    virtual size_t size() const = 0;
};

}

#endif
