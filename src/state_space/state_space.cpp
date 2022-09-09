#include "../../include/dlplan/state_space.h"

using namespace dlplan::core;


namespace dlplan::state_space {

StateSpace::StateSpace(
    std::shared_ptr<const core::InstanceInfo>&& instance_info,
    States&& states_by_index,
    StateIndex initial_state_index,
    AdjacencyMatrix&& adjacency_matrix,
    StateIndicesSet&& goal_state_indices)
    : m_instance_info(std::move(instance_info)),
      m_states_by_index(std::move(states_by_index)),
      m_initial_state_index(initial_state_index),
      m_goal_state_indices(std::move(goal_state_indices)) {
    // Compute flat forward transitions.
    for (int source_state_index = 0; source_state_index < get_num_states(); ++source_state_index) {
        m_forward_successor_state_indices_offsets.push_back(m_forward_successor_state_indices.size());
        for (int target_state_index : adjacency_matrix[source_state_index]) {
            m_forward_successor_state_indices.push_back(target_state_index);
        }
    }
    m_forward_successor_state_indices_offsets.push_back(m_forward_successor_state_indices.size());
    // Compute flat backward transitions.
    for (int target_state_index = 0; target_state_index < get_num_states(); ++target_state_index) {
        m_backward_successor_state_indices_offsets.push_back(m_backward_successor_state_indices.size());
        for (int source_state_index : adjacency_matrix[target_state_index]) {
            m_backward_successor_state_indices.push_back(source_state_index);
        }
    }
    m_backward_successor_state_indices_offsets.push_back(m_backward_successor_state_indices.size());
    // Compute goal distances.
    m_goal_distances = compute_distances(m_goal_state_indices, false);
    // Compute deadends.
    for (int state_index = 0; state_index < get_num_states(); ++state_index) {
        if (m_goal_distances[state_index] == INF) {
            m_deadend_state_indices.insert(state_index);
        }
    }
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
    int end = m_forward_successor_state_indices_offsets[source_state_index + 1];
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

int StateSpace::get_num_states() const {
    return m_states_by_index.size();
}

}
