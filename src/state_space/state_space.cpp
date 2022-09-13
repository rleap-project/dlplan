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
    for (int source_state_index = 0; source_state_index < successor_state_indices.size(); ++source_state_index) {
        for (int target_state_index : successor_state_indices[source_state_index]) {
            inverse_successor_state_indices[target_state_index].insert(source_state_index);
        }
    }
}

static core::States compute_states_by_index(const core::StatesSet& states) {
    // to avoid copying we sort state ptr by index of state
    std::vector<const core::State*> state_ptr_by_index(states.size(), nullptr);
    for (const auto& state : states) {
        int index = state.get_index();
        if (state_ptr_by_index[index] != nullptr) {
            throw std::runtime_error("StateSpace::compute_states_by_index - multiple states have the same index");
        }
        if (!utils::in_bounds(index, state_ptr_by_index)) {
            throw std::runtime_error("StateSpace::compute_states_by_index - state index out of bounds");
        }
        state_ptr_by_index[index] = &state;
    }
    core::States states_by_index;
    for (auto state_ptr : state_ptr_by_index) {
        states_by_index.push_back(*state_ptr);
    }
    return states_by_index;
}


StateSpace::StateSpace(
    std::shared_ptr<const InstanceInfo>&& instance_info,
    core::StatesSet&& states,
    StateIndex initial_state_index,
    AdjacencyList&& forward_successor_state_indices,
    StateIndicesSet&& goal_state_indices)
    : m_instance_info(std::move(instance_info)),
      m_states(std::move(states)),
      m_initial_state_index(initial_state_index),
      m_forward_successor_state_indices(std::move(forward_successor_state_indices)),
      m_goal_state_indices(std::move(goal_state_indices)) {
    // Perform input error checks.
    if (!std::all_of(m_states_by_index.begin(), m_states_by_index.end(),
        [this](const auto& state){ return state.get_instance_info() == this->get_instance_info(); })) {
        throw std::runtime_error("StateSpace::StateSpace - not all states come from the given InstanceInfo.");
    }
    if (!std::all_of(m_goal_state_indices.begin(), m_goal_state_indices.end(),
        [this](int goal_state_index){
            return utils::in_bounds(goal_state_index, m_goal_state_indices);
        })) {
        throw std::runtime_error("StateSpace::StateSpace - goal state index out of bounds.");
    }
    if (!utils::in_bounds(initial_state_index, m_states)) {
        throw std::runtime_error("StateSpace::StateSpace - initial state index out of bounds.");
    }
    if (m_forward_successor_state_indices.size() != m_states.size()) {
        throw std::runtime_error("StateSpace::StateSpace - source state index out of bounds.");
    }
    if (!std::all_of(m_forward_successor_state_indices.begin(), m_forward_successor_state_indices.end(),
        [this](const auto& successor_state_indices){
            std::all_of(successor_state_indices.begin(), successor_state_indices.end(),
            [this](int successor_state_index){
                utils::in_bounds(successor_state_index, m_states); })
        })) {
        throw std::runtime_error("StateSpace::StateSpace - target state index out of bounds.");
    }
    // Initialize additional attributes.
    m_states_by_index = compute_states_by_index(m_states);
    m_backward_successor_state_indices = compute_inverse_successor_state_indices(forward_successor_state_indices);
}

StateSpace::StateSpace(const StateSpace& other) = default;

StateSpace::StateSpace(
    const StateSpace& other,
    const StateIndicesSet& expanded_fragment,
    const StateIndicesSet& generated_fragment) {
    StateIndices old_to_new_state_index(other.m_states.size(), UNDEFINED);
    // set states and remap indices to 0,...,m-1
    for (const auto& state : other.m_states) {
        int old_state_index = state.get_index();
        if (generated_fragment.count(old_state_index)) {
            int new_state_index = m_states.size();
            core::State tmp_state = state;
            tmp_state.set_index(new_state_index);
            m_states.insert(tmp_state);
            m_states_by_index.push_back(tmp_state);
            old_to_new_state_index[old_state_index] = new_state_index;
        }
    }
    // set initial state index and remap
    if (!expanded_fragment.count(other.m_initial_state_index)) {
        m_initial_state_index = UNDEFINED;
    } else {
        m_initial_state_index = old_to_new_state_index[other.m_initial_state_index];
    }
    // set goal state indices and remap
    for (StateIndex goal_state_index : other.m_goal_state_indices) {
        if (generated_fragment.count(goal_state_index)) {
            m_goal_state_indices.insert(old_to_new_state_index[goal_state_index]);
        }
    }
    // set forward successor state indices and remap
}

StateSpace& StateSpace::operator=(const StateSpace& other) = default;

StateSpace::StateSpace(StateSpace&& other) = default;

StateSpace& StateSpace::operator=(StateSpace&& other) = default;

StateSpace::~StateSpace() = default;

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

void StateSpace::add_transition(int source_state_index, int target_state_index) {
    if (!(utils::in_bounds(source_state_index, m_states_by_index) &&
        utils::in_bounds(target_state_index, m_states_by_index))) {
        throw std::runtime_error("StateSpace::add_transition - source or target index out of bounds.");
    }
    m_forward_successor_state_indices[source_state_index].insert(target_state_index);
    m_backward_successor_state_indices[target_state_index].insert(source_state_index);
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

const core::State& StateSpace::add_state(const core::State& state) {

}

void StateSpace::add_transition(int source_state_index, int target_state_index) {
    if (!utils::in_bounds(source_state_index, m_states_by_index)) {
        throw std::runtime_error("StateSpace::add_transition - source_state_index out of bound.");
    }
    if (!utils::in_bounds(target_state_index, m_states_by_index)) {
        throw std::runtime_error("StateSpace::add_transition - target_state_index out of bound");
    }
    m_forward_successor_state_indices[source_state_index].insert(target_state_index);
    m_backward_successor_state_indices[target_state_index].insert(source_state_index);
}

void StateSpace::print() const {
    std::cout << "States:" << std::endl;
    for (const auto& state : m_states_by_index) {
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
}


StateSpaceInformation StateSpace::compute_state_space_information() const {
    auto goal_distances = compute_distances(m_goal_state_indices, false);
    StateIndicesSet deadend_state_indices;
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

const core::States& StateSpace::get_states_ref() const {
    return m_states_by_index;
}

const core::State& StateSpace::get_state_ref(StateIndex state_index) const {
    if (!utils::in_bounds(state_index, m_states_by_index)) {
        throw std::runtime_error("StateSpace - get_state_ref: state index out of range.");
    }
    return m_states_by_index[state_index];
}

const core::State& StateSpace::get_state_ref(core::State& state) const {
    auto result = m_states.find(state);
    if (result == m_states.end()) {
        throw std::runtime_error("StateSpace::get_state_ref - state does not exist in state space.");
    }
    return *result;
}

int StateSpace::get_num_states() const {
    return m_states_by_index.size();
}

StateIndex StateSpace::get_initial_state_index() const {
    return m_initial_state_index;
}

const StateIndicesSet& StateSpace::get_forward_successor_state_indices_ref(StateIndex state_index) const {
    return m_forward_successor_state_indices[state_index];
}

const StateIndicesSet& StateSpace::get_backward_successor_state_indices_ref(StateIndex state_index) const {
    return m_backward_successor_state_indices[state_index];
}

const StateIndicesSet& StateSpace::get_goal_state_indices_ref() const {
    return m_goal_state_indices;
}

std::shared_ptr<const InstanceInfo> StateSpace::get_instance_info() const {
    return m_instance_info;
}

}
