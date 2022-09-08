#ifndef DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_
#define DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_

#include "core.h"


namespace dlplan::state_space {

using StateIndex = int;


class StateSpace {
private:
    // For an efficient implementation we make use of indexing instead of shared_ptrs.
    core::States m_states;
    StateIndex m_initial_state_index;
    std::vector<StateIndex> m_forward_successor_state_indices;
    std::vector<StateIndex> m_forward_successor_state_indices_offsets;
    std::unordered_set<StateIndex> m_goal_state_indices;
    // backward successors can be derived from above knowledge but we precompute them for convenience.
    std::vector<StateIndex> m_backward_successor_state_indices;
    std::vector<StateIndex> m_backward_successor_state_indices_offsets;
    // deadends can be derived from above knowledge but we precompute them for convenience.
    std::unordered_set<StateIndex> m_deadend_state_indices;

public:
    StateSpace();

    /**
     * Getters.
     */
    const core::State& get_state_ref(int index) const;
};

/**
 * Reads a state space from file
 */
class StateSpaceReader {
};

}

#endif
