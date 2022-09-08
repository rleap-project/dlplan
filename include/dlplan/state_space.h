#ifndef DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_
#define DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_

#include "core.h"


namespace dlplan::state_space {

using StateIndex = int;


class StateSpace {
private:
    // For an efficient implementation we make use of indexing instead of shared_ptrs.
    core::States m_states_by_index;
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

    void for_each_state_index(std::function<void(int state_index)>&& function) const;

    void for_each_forward_successor_state_index(std::function<void(int state_index)>&& function, int source_state_index) const;

    /**
     * Getters.
     */
    const core::States& get_states_ref() const;
    const core::State& get_state_ref(int index) const;
    int get_num_states() const;
};

/**
 * Reads a state space from file
 */
class StateSpaceReader {
};

}

#endif
