#include "../../include/dlplan/state_space.h"

#include <deque>
#include <iostream>
#include <stdexcept>

#include "../utils/collections.h"
#include "../utils/memory.h"
#include "../utils/set_operators.h"


using namespace dlplan::core;


namespace dlplan::state_space {

static AdjacencyList compute_inverse_successor_state_indices(const AdjacencyList& successor_state_indices) {
    AdjacencyList inverse_successor_state_indices(successor_state_indices.size());
    for (const auto& pair : successor_state_indices) {
        StateIndex source_state_index = pair.first;
        for (auto target_state_index : pair.second) {
            inverse_successor_state_indices[target_state_index].insert(source_state_index);
        }
    }
    return inverse_successor_state_indices;
}

StateSpace::StateSpace(
    std::shared_ptr<const InstanceInfo>&& instance_info,
    std::unordered_map<StateIndex, core::State>&& state_index_to_state,
    StateIndex initial_state_index,
    AdjacencyList&& forward_successor_state_indices,
    StateIndices&& goal_state_indices)
    : m_instance_info(std::move(instance_info)),
      m_state_index_to_state(std::move(state_index_to_state)),
      m_initial_state_index(initial_state_index),
      m_forward_successor_state_indices(std::move(forward_successor_state_indices)),
      m_goal_state_indices(std::move(goal_state_indices)) {
    // Perform input error checks.
    if (!std::all_of(m_state_index_to_state.begin(), m_state_index_to_state.end(),
        [this](const auto& pair){ return pair.second.get_instance_info() == this->get_instance_info(); })) {
        throw std::runtime_error("StateSpace::StateSpace - not all states come from the given InstanceInfo.");
    }
    if (!std::all_of(m_goal_state_indices.begin(), m_goal_state_indices.end(),
        [this](int goal_state_index){
            return utils::in_bounds(goal_state_index, m_goal_state_indices);
        })) {
        throw std::runtime_error("StateSpace::StateSpace - goal state index out of bounds.");
    }
    if (!m_state_index_to_state.count(initial_state_index)) {
        throw std::runtime_error("StateSpace::StateSpace - initial state index out of bounds.");
    }
    if (!std::all_of(m_forward_successor_state_indices.begin(), m_forward_successor_state_indices.end(),
        [this](const auto& pair) {
            return m_state_index_to_state.count(pair.first);
        })) {
        throw std::runtime_error("StateSpace::StateSpace - source state index out of bounds.");
    }
    if (!std::all_of(m_forward_successor_state_indices.begin(), m_forward_successor_state_indices.end(),
        [this](const auto& pair){
            return std::all_of(pair.second.begin(), pair.second.end(),
            [this](int successor_state_index){
                return m_state_index_to_state.count(successor_state_index);
            });
        })) {
        throw std::runtime_error("StateSpace::StateSpace - target state index out of bounds.");
    }
    m_backward_successor_state_indices = compute_inverse_successor_state_indices(forward_successor_state_indices);
}

StateSpace::StateSpace(const StateSpace& other) = default;

StateSpace::StateSpace(
    const StateSpace& other,
    const StateIndices& expanded_fragment,
    const StateIndices& generated_fragment) {
    // set state_index_to_state
    for (const auto& pair : other.m_state_index_to_state) {
        auto& state = pair.second;
        int state_index = state.get_index();
        if (generated_fragment.count(state_index)) {
            m_state_index_to_state.emplace(state_index, state);
        }
    }
    // set initial_state_index
    if (!expanded_fragment.count(other.m_initial_state_index)) {
        m_initial_state_index = UNDEFINED;
    } else {
        m_initial_state_index = other.m_initial_state_index;
    }
    // set goal_state_indices
    for (StateIndex goal_state_index : other.m_goal_state_indices) {
        if (generated_fragment.count(goal_state_index)) {
            m_goal_state_indices.insert(goal_state_index);
        }
    }
    // TODO: set forward_successor_state_indices
}

StateSpace& StateSpace::operator=(const StateSpace& other) = default;

StateSpace::StateSpace(StateSpace&& other) = default;

StateSpace& StateSpace::operator=(StateSpace&& other) = default;

StateSpace::~StateSpace() = default;

Distances StateSpace::compute_distances(const StateIndices& state_indices, bool forward) const {
    Distances distances;
    std::deque<StateIndex> queue;
    for (auto state_index : state_indices) {
        queue.push_back(state_index);
        distances[state_index] = 0;
    }
    AdjacencyList successor_state_indices = (forward) ? m_forward_successor_state_indices : m_backward_successor_state_indices;
    while (!queue.empty()) {
        int current_state_index = queue.front();
        queue.pop_front();
        for (int successor_state_index : successor_state_indices[current_state_index]) {
            if (!distances.count(successor_state_index)) {
                distances[successor_state_index] = distances[current_state_index] + 1;
                queue.push_back(successor_state_index);
            }
        }
    }
    return distances;
}

const core::State& StateSpace::add_state(const core::State& state) {

}

void StateSpace::add_transition(int source_state_index, int target_state_index) {
    if (!m_state_index_to_state.count(source_state_index)) {
        throw std::runtime_error("StateSpace::add_transition - source_state_index out of bound.");
    }
    if (!m_state_index_to_state.count(target_state_index)) {
        throw std::runtime_error("StateSpace::add_transition - target_state_index out of bound");
    }
    m_forward_successor_state_indices[source_state_index].insert(target_state_index);
    m_backward_successor_state_indices[target_state_index].insert(source_state_index);
}

void StateSpace::print() const {
    std::cout << "States:" << std::endl;
    for (const auto& pair : m_state_index_to_state) {
        std::cout << "    " << pair.second.str() << std::endl;
    }
    std::cout << "Transitions:" << std::endl;
    for_each_state_index(
        [this](const int state_index){
            std::cout << "    " << state_index << ": ";
            for_each_forward_successor_state_index(
                [state_index](const int successor_state_index){
                    std::cout << successor_state_index << " ";
                }, state_index);
            std::cout << std::endl;
        }
    );
    std::cout << "Goal state indices: ";
    for (const auto goal_state_index : m_goal_state_indices) {
        std::cout << goal_state_index << " ";
    }
    std::cout << std::endl;
}


StateSpaceInformation StateSpace::compute_state_space_information() const {
    auto goal_distances = compute_distances(m_goal_state_indices, false);
    StateIndices deadend_state_indices;
    for (int state_index = 0; state_index < get_num_states(); ++state_index) {
        if (goal_distances[state_index] == INF) {
            deadend_state_indices.insert(state_index);
        }
    }
    return StateSpaceInformation(m_initial_state_index, std::move(goal_distances), std::move(deadend_state_indices));
}

void StateSpace::set_initial_state_index(StateIndex state_index) {
    m_initial_state_index = state_index;
}

const std::unordered_map<StateIndex, core::State>& StateSpace::get_state_index_to_state_ref() const {
    return m_state_index_to_state;
}

const core::State& StateSpace::get_state_ref(StateIndex state_index) const {
    const auto& result = m_state_index_to_state.find(state_index);
    if (result == m_state_index_to_state.end()) {
        throw std::runtime_error("StateSpace - get_state_ref: state index out of range.");
    }
    return result->second;
}

int StateSpace::get_num_states() const {
    return m_state_index_to_state.size();
}

StateIndex StateSpace::get_initial_state_index() const {
    return m_initial_state_index;
}

const StateIndices& StateSpace::get_forward_successor_state_indices_ref(StateIndex state_index) const {
    const auto& result = m_forward_successor_state_indices.find(state_index);
    if (result == m_forward_successor_state_indices.end()) {
        throw std::runtime_error("StateSpace::get_forward_successor_state_indices_ref - state index out of bounds.");
    }
    return result->second;
}

const StateIndices& StateSpace::get_backward_successor_state_indices_ref(StateIndex state_index) const {
    const auto& result = m_backward_successor_state_indices.find(state_index);
    if (result == m_backward_successor_state_indices.end()) {
        throw std::runtime_error("StateSpace::get_backward_successor_state_indices_ref - state index out of bounds.");
    }
    return result->second;
}

const StateIndices& StateSpace::get_goal_state_indices_ref() const {
    return m_goal_state_indices;
}

std::shared_ptr<const InstanceInfo> StateSpace::get_instance_info() const {
    return m_instance_info;
}

}
