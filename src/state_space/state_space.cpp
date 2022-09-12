#include "../../include/dlplan/state_space.h"

#include <deque>
#include <iostream>

#include "../utils/collections.h"
#include "../utils/memory.h"
#include "../utils/set_operators.h"


using namespace dlplan::core;


namespace dlplan::state_space {

StateSpace::StateSpace(
    std::shared_ptr<const InstanceInfo>&& instance_info,
    States&& states,
    StateIndex initial_state_index,
    AdjacencyList&& adjacency_list,
    StateIndicesSet&& goal_state_indices)
    : m_instance_info(std::move(instance_info)),
      m_states(std::move(states)),
      m_initial_state_index(initial_state_index),
      m_goal_state_indices(std::move(goal_state_indices)) {
    // Change adjacency list to more compact representation.
    for (int source_state_index = 0; source_state_index < get_num_states(); ++source_state_index) {
        m_forward_successor_state_indices_offsets.push_back(m_forward_successor_state_indices.size());
        for (int target_state_index : adjacency_list[source_state_index]) {
            m_forward_successor_state_indices.push_back(target_state_index);
        }
    }
    m_forward_successor_state_indices_offsets.push_back(m_forward_successor_state_indices.size());
    m_forward_successor_state_indices_offsets.push_back(m_forward_successor_state_indices.size());
    initialize_backward_successors();
    initialize_goal_distances_and_deadends();
}

StateSpace::StateSpace(const StateSpace& other)
    : m_instance_info(std::make_shared<core::InstanceInfo>(*other.m_instance_info)),
      m_initial_state_index(other.m_initial_state_index),
      m_forward_successor_state_indices(other.m_forward_successor_state_indices),
      m_forward_successor_state_indices_offsets(other.m_forward_successor_state_indices_offsets),
      m_goal_state_indices(other.m_goal_state_indices),
      m_backward_successor_state_indices(other.m_backward_successor_state_indices),
      m_backward_successor_state_indices_offsets(other.m_backward_successor_state_indices_offsets),
      m_goal_distances(other.m_goal_distances),
      m_deadend_state_indices(other.m_deadend_state_indices) {
    m_states.reserve(other.get_num_states());
    for (const auto& state : other.get_states_ref()) {
        m_states.push_back(core::State(m_instance_info, state.get_atom_idxs(), state.get_index()));
    }
}

StateSpace& StateSpace::operator=(const StateSpace& other) {
    if (this != &other) {
        m_instance_info = std::make_shared<core::InstanceInfo>(*other.m_instance_info);
        m_states.reserve(other.get_num_states());
        for (const auto& state : other.get_states_ref()) {
            m_states.push_back(core::State(m_instance_info, state.get_atom_idxs(), state.get_index()));
        }
        m_initial_state_index = other.m_initial_state_index;
        m_forward_successor_state_indices = other.m_forward_successor_state_indices;
        m_forward_successor_state_indices_offsets = other.m_forward_successor_state_indices_offsets;
        m_goal_state_indices = other.m_goal_state_indices;
        m_backward_successor_state_indices = other.m_backward_successor_state_indices;
        m_backward_successor_state_indices_offsets = other.m_backward_successor_state_indices_offsets;
        m_goal_distances = other.m_goal_distances;
        m_deadend_state_indices = other.m_deadend_state_indices;
    }
    return *this;
}

StateSpace::StateSpace(StateSpace&& other) = default;

StateSpace& StateSpace::operator=(StateSpace&& other) = default;

StateSpace::~StateSpace() = default;

void StateSpace::initialize_backward_successors() {
    // Compute backward adjacency list.
    AdjacencyList backward_adjacency_list(get_num_states());
    for_each_state_index(
    [this, &backward_adjacency_list](int source_state_index){
        this->for_each_forward_successor_state_index(
        [&backward_adjacency_list, source_state_index](int target_state_index){
            backward_adjacency_list[target_state_index].push_back(source_state_index);
        }, source_state_index);
    });
    // Change adjacency list to more compact representation.
    // TODO: memory is not necessarily free-ed so we might want to implement a swap.
    utils::free_memory(m_backward_successor_state_indices);
    utils::free_memory(m_backward_successor_state_indices_offsets);
    for (int target_state_index = 0; target_state_index < get_num_states(); ++target_state_index) {
        m_backward_successor_state_indices_offsets.push_back(m_backward_successor_state_indices.size());
        for (int source_state_index : backward_adjacency_list[target_state_index]) {
            m_backward_successor_state_indices.push_back(source_state_index);
        }
    }
    m_backward_successor_state_indices_offsets.push_back(m_backward_successor_state_indices.size());
}

void StateSpace::initialize_goal_distances_and_deadends() {
    // Compute goal distances.
    m_goal_distances = compute_distances(m_goal_state_indices, false);
    // Compute deadends.
    utils::free_memory(m_deadend_state_indices);
    for (int state_index = 0; state_index < get_num_states(); ++state_index) {
        if (m_goal_distances[state_index] == INF) {
            m_deadend_state_indices.insert(state_index);
        }
    }
}

StateIndices StateSpace::prune_states(const StateIndicesSet& state_indices) {
    if (state_indices.count(m_initial_state_index)) {
        throw std::runtime_error("StateSpace::prune_states - pruning the initial state is not allowed.");
    }
    // Compute states that make it into the pruned state space.
    StateIndices new_to_old_state_indices;
    StateIndices old_to_new_state_indices(get_num_states(), UNDEFINED);
    States new_states;
    for (auto& state : m_states) {
        if (state_indices.count(state.get_index())) {
            continue;
        }
        int new_state_index = new_states.size();
        int old_state_index = state.get_index();
        new_to_old_state_indices.push_back(old_state_index);
        old_to_new_state_indices[old_state_index] = new_state_index;
        state.set_index(new_state_index);
        new_states.push_back(state);
    }
    // Compute forward transition that make it into the pruned state space.
    StateIndices new_forward_successor_state_indices;
    StateIndices new_forward_successor_state_indices_offsets;
    for_each_state_index(
    [this, &state_indices, &old_to_new_state_indices, &new_forward_successor_state_indices, &new_forward_successor_state_indices_offsets](int source_state_index){
        if (state_indices.count(source_state_index)) {
            return;
        }
        new_forward_successor_state_indices_offsets.push_back(new_forward_successor_state_indices.size());
        this->for_each_forward_successor_state_index(
        [&state_indices, &old_to_new_state_indices, &new_forward_successor_state_indices, source_state_index](int target_state_index){
            if (state_indices.count(target_state_index)) {

                return;
            }
            // transition exists in pruned transition system
            new_forward_successor_state_indices.push_back(old_to_new_state_indices[target_state_index]);
        }, source_state_index);
    });
    new_forward_successor_state_indices_offsets.push_back(new_forward_successor_state_indices.size());
    m_forward_successor_state_indices = new_forward_successor_state_indices;
    m_forward_successor_state_indices_offsets = new_forward_successor_state_indices_offsets;
    // Compute goal states
    m_goal_state_indices = utils::set_difference(m_goal_state_indices, state_indices);
    // Reinitialize derived information.
    initialize_backward_successors();
    initialize_goal_distances_and_deadends();
    return new_to_old_state_indices;
}

Distances StateSpace::compute_distances(const StateIndicesSet& state_indices, bool forward) const {
    Distances distances(get_num_states(), INF);
    std::deque<StateIndex> queue;
    for (auto state_index : state_indices) {
        queue.push_back(state_index);
        distances[state_index] = 0;
    }
    auto distance_update_func =
        [&distances, &queue](int current_state_index, int successor_state_index){
        if (distances[successor_state_index] == INF) {
            distances[successor_state_index] = distances[current_state_index] + 1;
            queue.push_back(successor_state_index);
        }
    };
    while (!queue.empty()) {
        int current_state_index = queue.front();
        queue.pop_front();
        if (forward) {
            for_each_forward_successor_state_index(
                [&distances, &queue, &distance_update_func, current_state_index](int successor_state_index){
                    distance_update_func(current_state_index, successor_state_index);
                },
                current_state_index);
        } else {
            for_each_backward_successor_state_index(
                [&distances, &queue, &distance_update_func, current_state_index](int successor_state_index){
                    distance_update_func(current_state_index, successor_state_index);
                },
                current_state_index);
        }
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
        function(m_forward_successor_state_indices[start]);
    }
}

void StateSpace::for_each_backward_successor_state_index(std::function<void(int state_index)>&& function, int state_index) const {
    int start = m_backward_successor_state_indices_offsets[state_index];
    int end = m_backward_successor_state_indices_offsets[state_index + 1];
    for (; start < end; ++start) {
        function(m_backward_successor_state_indices[start]);
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

bool StateSpace::is_solvable() const {
    return !is_deadend(m_initial_state_index);
}

bool StateSpace::is_trivially_solvable() const {
    return std::all_of(m_states.begin(), m_states.end(),
    [this](const auto& state){
        return m_goal_state_indices.count(state.get_index());
    });
}

void StateSpace::print() const {
    std::cout << "States:" << std::endl;
    for (const auto& state : m_states) {
        std::cout << "    " << state.str() << std::endl;
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
    std::cout << "Deadend state indices: ";
    for (const auto deadend_state_index : m_deadend_state_indices) {
        std::cout << deadend_state_index << " ";
    }
    std::cout << std::endl;
}

void StateSpace::set_initial_state_index(int state_index) {
    m_initial_state_index = state_index;
    initialize_goal_distances_and_deadends();
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

StateIndex StateSpace::get_initial_state_index() const {
    return m_initial_state_index;
}

StateIndices StateSpace::get_forward_successor_state_indices(int state_index) const {
    return StateIndices(
        &m_forward_successor_state_indices[m_forward_successor_state_indices_offsets[state_index]],
        &m_forward_successor_state_indices[m_forward_successor_state_indices_offsets[state_index + 1]]);
}

StateIndices StateSpace::get_backward_successor_state_indices(int state_index) const {
    return StateIndices(
        &m_backward_successor_state_indices[m_backward_successor_state_indices_offsets[state_index]],
        &m_backward_successor_state_indices[m_backward_successor_state_indices_offsets[state_index + 1]]);
}

const StateIndicesSet& StateSpace::get_goal_state_indices_ref() const {
    return m_goal_state_indices;
}

const StateIndicesSet& StateSpace::get_deadend_state_indices_ref() const {
    return m_deadend_state_indices;
}

const Distances& StateSpace::get_goal_distances_ref() const {
    return m_goal_distances;
}

std::shared_ptr<const InstanceInfo> StateSpace::get_instance_info() const {
    return m_instance_info;
}

}
