#include "../../include/dlplan/state_space.h"

using namespace dlplan::core;


namespace dlplan::state_space {

StateSpace::StateSpace(
    States&& states,
    StateIndex initial_state_index,
    StateIndices&& forward_successor_state_indices,
    StateIndices&& forward_successor_state_indices_offsets,
    StateIndicesSet&& m_goal_state_indices) {
    // TODO: implement
}

StateSpace::StateSpace(const StateSpace& other) = default;

StateSpace& StateSpace::operator=(const StateSpace& other) = default;

StateSpace::StateSpace(StateSpace&& other) = default;

StateSpace& StateSpace::operator=(StateSpace&& other) = default;

StateSpace::~StateSpace() = default;

void StateSpace::for_each_state_index(std::function<void(int state_index)>&& function) const {
    for (int state_index = 0; state_index < get_num_states(); ++state_index) {
        function(state_index);
    }
}

void StateSpace::for_each_forward_successor_state_index(std::function<void(int state_index)>&& function, int source_state_index) const {
    int start = m_forward_successor_state_indices_offsets[source_state_index];
    int end = ((source_state_index + 1) < get_num_states())
        ? m_forward_successor_state_indices_offsets[source_state_index + 1]
        : m_forward_successor_state_indices.size();
    for (; start < end; ++start) {
        function(start);
    }
}

bool StateSpace::is_goal(StateIndex state_index) const {
    return m_goal_state_indices.count(state_index);
}

bool StateSpace::is_nongoal(StateIndex state_index) const {
    return !is_goal(state_index);
}

bool StateSpace::is_deadend(StateIndex state_index) const {
    return m_deadend_state_indices.count(state_index);
}

bool StateSpace::is_alive(StateIndex state_index) const {
    return !(is_goal(state_index) || is_deadend(state_index));
}

}
