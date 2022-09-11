#include "../../include/dlplan/state_space.h"

#include <deque>
#include <iostream>

#include "../utils/collections.h"


using namespace dlplan::core;


namespace dlplan::state_space {

StateSpace::StateSpace(
    std::shared_ptr<const core::InstanceInfo>&& instance_info,
    States&& states,
    StateIndex initial_state_index,
    AdjacencyList&& adjacency_matrix,
    StateIndicesSet&& goal_state_indices)
    : m_instance_info(std::move(instance_info)),
      m_states(std::move(states)),
      m_initial_state_index(initial_state_index),
      m_goal_state_indices(std::move(goal_state_indices)) {
    m_states_offsets = StateIndices(m_states.size());
    for (const auto& state : m_states) {
        m_states_offsets.push_back(state.get_index());
    }
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
    m_goal_distances = compute_distances_to_states(m_goal_state_indices);
    // Compute deadends.
    for (int state_index = 0; state_index < get_num_states(); ++state_index) {
        if (m_goal_distances[state_index] == INF) {
            m_deadend_state_indices.insert(state_index);
        }
    }
}

StateSpace::StateSpace(const StateSpace& other)
    : m_instance_info(std::make_shared<core::InstanceInfo>(*other.m_instance_info)),
      m_initial_state_index(other.m_initial_state_index),
      m_forward_successor_state_indices(other.m_forward_successor_state_indices),
      m_forward_successor_state_indices_offsets(other.m_forward_successor_state_indices_offsets),
      m_backward_successor_state_indices(other.m_backward_successor_state_indices),
      m_backward_successor_state_indices_offsets(other.m_backward_successor_state_indices_offsets) {
    m_states.reserve(other.get_num_states());
    for (const auto& state : other.get_states_ref()) {
        m_states.push_back(core::State(m_instance_info, state.get_atom_idxs()));
    }
}

StateSpace& StateSpace::operator=(const StateSpace& other) {
    if (this != &other) {
        m_instance_info = std::make_shared<core::InstanceInfo>(*other.m_instance_info);
        m_initial_state_index = other.m_initial_state_index;
        m_forward_successor_state_indices = other.m_forward_successor_state_indices;
        m_forward_successor_state_indices_offsets = other.m_forward_successor_state_indices_offsets;
        m_backward_successor_state_indices = other.m_backward_successor_state_indices;
        m_backward_successor_state_indices_offsets = other.m_backward_successor_state_indices_offsets;
        m_states.reserve(other.get_num_states());
        for (const auto& state : other.get_states_ref()) {
            m_states.push_back(core::State(m_instance_info, state.get_atom_idxs()));
        }
    }
    return *this;
}

StateSpace::StateSpace(StateSpace&& other) = default;

StateSpace& StateSpace::operator=(StateSpace&& other) = default;

StateSpace::~StateSpace() = default;

Distances StateSpace::compute_distances_to_states(const StateIndicesSet& state_indices) {
    Distances distances(get_num_states(), INF);
    std::deque<StateIndex> queue;
    for (auto state_index : state_indices) {
        queue.push_back(state_index);
        distances[state_index] = 0;
    }
    while (!queue.empty()) {
        int current_state_index = queue.front();
        queue.pop_front();
        for_each_backward_successor_state_index(
            [&distances, &queue, current_state_index](int successor_state_index){
                if (distances[successor_state_index] == INF) {
                    distances[successor_state_index] = distances[current_state_index] + 1;
                    queue.push_back(successor_state_index);
                }
            },
            current_state_index);
    }
    return distances;
}

void StateSpace::for_each_state_index(std::function<void(int state_index)>&& function) const {
    for (int state_index = 0; state_index < get_num_states(); ++state_index) {
        function(state_index);
    }
}

void StateSpace::for_each_forward_successor_state_index(std::function<void(int state_index)>&& function, int state_index) const {
    int start = m_forward_successor_state_indices_offsets[state_index];
    int end = m_forward_successor_state_indices_offsets[state_index + 1];
    for (; start < end; ++start) {
        function(start);
    }
}

void StateSpace::for_each_backward_successor_state_index(std::function<void(int state_index)>&& function, int state_index) const {
    int start = m_backward_successor_state_indices_offsets[state_index];
    int end = m_backward_successor_state_indices_offsets[state_index + 1];
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

const core::States& StateSpace::get_states_ref() const {
    return m_states;
}

const core::State& StateSpace::get_state_ref(int index) const {
    if (!utils::in_bounds(index, m_states)) {
        throw std::runtime_error("StateSpace - get_state_ref: state index out of range.");
    }
    return m_states[index];
}

int StateSpace::get_num_states() const {
    return m_states.size();
}

const Distances& StateSpace::get_goal_distances_ref() const {
    return m_goal_distances;
}

}
