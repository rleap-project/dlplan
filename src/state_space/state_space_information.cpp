#include "../../include/dlplan/state_space.h"


namespace dlplan::state_space {

bool StateSpaceInformation::is_goal(StateIndex state_index) const {
    auto result = m_goal_distances.find(state_index);
    if (result == m_goal_distances.end()) {
        throw std::runtime_error("StateSpaceInformation::is_goal - state index out of bounds");
    }
    return result->first == 0;
}

bool StateSpaceInformation::is_nongoal(StateIndex state_index) const {
    return !is_goal(state_index);
}

bool StateSpaceInformation::is_deadend(StateIndex state_index) const {
    return m_deadend_state_indices.count(state_index);
}

bool StateSpaceInformation::is_alive(StateIndex state_index) const {
    return !(is_goal(state_index) || is_deadend(state_index));
}

bool StateSpaceInformation::is_solvable() const {
    return !is_deadend(m_initial_state_index);
}

bool StateSpaceInformation::is_trivially_solvable() const {
    auto result = m_goal_distances.find(m_initial_state_index);
    if (result == m_goal_distances.end()) {
        throw std::runtime_error("StateSpaceInformation::is_trivially_solvable - state index out of bounds");
    }
    return result->first == 0;
}

const StateIndices& StateSpaceInformation::get_deadend_state_indices_ref() const {
    return m_deadend_state_indices;
}

const Distances& StateSpaceInformation::get_goal_distances_ref() const {
    return m_goal_distances;
}

}
