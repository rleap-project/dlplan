#include "../../include/dlplan/state_space.h"

#include <deque>
#include <iostream>
#include <stdexcept>

#include "../utils/collections.h"
#include "../utils/memory.h"
#include "../utils/set_operators.h"


using namespace dlplan::core;


namespace dlplan::state_space {

StateSpace::StateSpace(
    std::shared_ptr<const InstanceInfo>&& instance_info,
    States&& states_by_index,
    StateIndex initial_state_index,
    AdjacencyList&& forward_successor_state_indices,
    StateIndicesSet&& goal_state_indices)
    : m_instance_info(std::move(instance_info)),
      m_states_by_index(std::move(states_by_index)),
      m_initial_state_index(initial_state_index),
      m_forward_successor_state_indices(std::move(forward_successor_state_indices)),
      m_goal_state_indices(std::move(goal_state_indices)) {
    if (!std::all_of(m_states_by_index.begin(), m_states_by_index.end(),
        [this](const auto& state){ return state.get_instance_info() == this->get_instance_info(); })) {
        throw std::runtime_error("StateSpace::StateSpace - not all states come from the given InstanceInfo.");
    }
    // TODO: add more error checks.
    initialize_backward_successors();
    initialize_goal_distances_and_deadends();
}

StateSpace::StateSpace(const StateSpace& other)
    : m_instance_info(std::make_shared<core::InstanceInfo>(*other.m_instance_info)),
      m_initial_state_index(other.m_initial_state_index),
      m_forward_successor_state_indices(other.m_forward_successor_state_indices),
      m_goal_state_indices(other.m_goal_state_indices),
      m_backward_successor_state_indices(other.m_backward_successor_state_indices),
      m_goal_distances(other.m_goal_distances),
      m_deadend_state_indices(other.m_deadend_state_indices) {
    m_states_by_index.reserve(other.get_num_states());
    if (!utils::in_bounds(m_initial_state_index, other.m_states_by_index)) {
        throw std::runtime_error("StateSpace::StateSpace - the initial state must be part of the state space.");
    }
    // TODO: check if states are pairwise different.
    for (const auto& state : other.get_states_ref()) {
        m_states_by_index.push_back(core::State(m_instance_info, state.get_atom_idxs(), state.get_index()));
    }
}

StateSpace::StateSpace(const StateSpace& other, const StateIndices& state_indices_fragment, int initial_state_index, const StateIndicesSet& goal_state_indices) {
    StateIndicesSet state_indices_set(state_indices_fragment.begin(), state_indices_fragment.end());
    if (state_indices_fragment.size() != state_indices_set.size()) {
        throw std::runtime_error("StateSpace::StateSpace - the state indices fragment must not contain duplicate states.");
    }
    if (!state_indices_set.count(initial_state_index)) {
        throw std::runtime_error("StateSpace::StateSpace - the initial state must be part of the state space fragment.");
    }
    // TODO: check if states are pairwise different.
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
        m_goal_state_indices = other.m_goal_state_indices;
        m_backward_successor_state_indices = other.m_backward_successor_state_indices;
        m_goal_distances = other.m_goal_distances;
        m_deadend_state_indices = other.m_deadend_state_indices;
    }
    return *this;
}

StateSpace::StateSpace(StateSpace&& other) = default;

StateSpace& StateSpace::operator=(StateSpace&& other) = default;

StateSpace::~StateSpace() = default;

void StateSpace::initialize_backward_successors() {
    // Change adjacency list to more compact representation.
    utils::free_memory(m_backward_successor_state_indices);
    m_backward_successor_state_indices = AdjacencyList(get_num_states());
    for (int source_state_index = 0; source_state_index < get_num_states(); ++source_state_index) {
        for (int target_state_index : m_forward_successor_state_indices[source_state_index]) {
            m_backward_successor_state_indices[target_state_index].push_back(source_state_index);
        }
    }
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

const core::State& StateSpace::add_state(const core::State& state) {
    auto tmp_state = state;
    tmp_state.set_index(m_states.size());
    auto exists = m_states.insert(tmp_state);
    if (!exists.second) {
        m_states_by_index.push_back(*exists.first);
    }
    return *exists.first;
}

void StateSpace::add_transition(int source_state_index, int target_state_index) {
    if (!(utils::in_bounds(source_state_index, m_states_by_index) &&
        utils::in_bounds(target_state_index, m_states_by_index))) {
        throw std::runtime_error("StateSpace::add_transition - source or target index out of bounds.");
    }
    m_forward_successor_state_indices[source_state_index].push_back(target_state_index);
    m_backward_successor_state_indices[target_state_index].push_back(source_state_index);
}

Distances StateSpace::compute_distances(const StateIndicesSet& state_indices, bool forward) const {
    Distances distances(get_num_states(), INF);
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
            if (distances[successor_state_index] == INF) {
                distances[successor_state_index] = distances[current_state_index] + 1;
                queue.push_back(successor_state_index);
            }
        }
    }
    return distances;
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
    return m_goal_state_indices.count(m_initial_state_index);
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
    return m_states_by_index;
}

const core::State& StateSpace::get_state_ref(int index) const {
    if (!utils::in_bounds(index, m_states)) {
        throw std::runtime_error("StateSpace - get_state_ref: state index out of range.");
    }
    return m_states_by_index[index];
}

int StateSpace::get_num_states() const {
    return m_states.size();
}

StateIndex StateSpace::get_initial_state_index() const {
    return m_initial_state_index;
}

const StateIndices& StateSpace::get_forward_successor_state_indices_ref(int state_index) const {
    return m_forward_successor_state_indices[state_index];
}

const StateIndices& StateSpace::get_backward_successor_state_indices_ref(int state_index) const {
    return m_backward_successor_state_indices[state_index];
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
