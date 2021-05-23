#ifndef DLP_SRC_CORE_ELEMENT_H_
#define DLP_SRC_CORE_ELEMENT_H_

#include "types.h"

namespace dlp {
namespace core {

template<typename T>
class Element {
protected:
    std::shared_ptr<TaskInfo> m_task_info;
    bool m_goal;
    T m_result;
    const Index_Vec *m_state_atoms;

protected:
    virtual T evaluate_impl(const Index_Vec& atoms) = 0;

public:
    Element(std::shared_ptr<TaskInfo> task_info, bool goal)
    : m_task_info(task_info), m_goal(goal) { }

    /**
     * Initializes the element if defined to be evaluated in the goal only.
     */
    void initialize() {
        if (m_goal) {
            evaluate_impl(m_task_info->goal_atom_idxs());
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
     * Returns the complexity of the element.
     */
    virtual unsigned complexity() const = 0;

    /**
     * The size of the result.
     */
    virtual size_t size() const = 0;

    /**
     * Getters.
     */
    virtual bool goal() const {
        return m_goal;
    }
};

}
}

#endif
