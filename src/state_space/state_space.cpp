#include "../../include/dlplan/state_space.h"

#include "../utils/collections.h"
#include "../utils/memory.h"
#include "../utils/set_operators.h"

#include <algorithm>
#include <deque>
#include <iostream>
#include <stdexcept>
#include <sstream>


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
    core::StatesSet&& states,
    StateIndex initial_state_index,
    AdjacencyList&& forward_successor_state_indices,
    StateIndicesSet&& goal_state_indices)
    : m_instance_info(std::move(instance_info)),
      m_states(std::move(states)),
      m_initial_state_index(initial_state_index),
      m_forward_successor_state_indices(std::move(forward_successor_state_indices)),
      m_goal_state_indices(std::move(goal_state_indices)) {
    // assert states
    if (!std::all_of(m_states.begin(), m_states.end(),
        [this](const auto& state){ return &state.get_instance_info_ref() == &this->get_instance_info_ref(); })) {
        throw std::runtime_error("StateSpace::StateSpace - not all states come from the given InstanceInfo.");
    }
    // compute state indices
    std::transform(m_states.begin(), m_states.end(), std::inserter(m_state_indices, m_state_indices.begin()), [](const auto& state){ return state.get_index(); });
    // assert goals
    if (!std::all_of(m_goal_state_indices.begin(), m_goal_state_indices.end(),
        [this](StateIndex goal_state){
            return m_state_indices.count(goal_state);
        })) {
        throw std::runtime_error("StateSpace::StateSpace - goal state index out of bounds.");
    }
    // assert initial state
    if (!m_state_indices.count(m_initial_state_index)) {
        throw std::runtime_error("StateSpace::StateSpace - initial state index out of bounds." + std::to_string(m_initial_state_index));
    }
    // assert forward successors
    if (!std::all_of(m_forward_successor_state_indices.begin(), m_forward_successor_state_indices.end(),
        [this](const auto& pair) {
            return m_state_indices.count(pair.first);
        })) {
        throw std::runtime_error("StateSpace::StateSpace - source state index out of bounds.");
    }
    if (!std::all_of(m_forward_successor_state_indices.begin(), m_forward_successor_state_indices.end(),
        [this](const auto& pair){
            return std::all_of(pair.second.begin(), pair.second.end(),
            [this](StateIndex successor){
                return m_state_indices.count(successor);
            });
        })) {
        throw std::runtime_error("StateSpace::StateSpace - target state index out of bounds.");
    }
    // compute backward successors
    m_backward_successor_state_indices = compute_inverse_successor_state_indices(m_forward_successor_state_indices);
}

StateSpace::StateSpace(const StateSpace& other) = default;

StateSpace::StateSpace(
    const StateSpace& other,
    const StateIndicesSet& expanded_fragment,
    const StateIndicesSet& generated_fragment)
    : m_instance_info(other.m_instance_info) {
    // set state_index_to_state
    for (const auto& state : other.m_states) {
        StateIndex state_index = state.get_index();
        if (generated_fragment.count(state_index)) {
            m_states.insert(state);
            m_state_indices.insert(state_index);
        }
    }
    // set initial_state_index
    if (!generated_fragment.count(other.m_initial_state_index)) {
        m_initial_state_index = UNDEFINED;
    } else {
        m_initial_state_index = other.m_initial_state_index;
    }
    // set goal_state_indices
    for (StateIndex goal_state : other.m_goal_state_indices) {
        if (generated_fragment.count(goal_state)) {
            m_goal_state_indices.insert(goal_state);
        }
    }
    // set forward_successor_state_indices
    for (const auto& pair : other.m_forward_successor_state_indices) {
        StateIndex source = pair.first;
        if (expanded_fragment.count(source)) {
            const auto& successors = pair.second;
            for (StateIndex successor : successors) {
                if (generated_fragment.count(successor)) {
                    m_forward_successor_state_indices[source].insert(successor);
                }
            }
        }
    }
    m_backward_successor_state_indices = compute_inverse_successor_state_indices(m_forward_successor_state_indices);
}

StateSpace& StateSpace::operator=(const StateSpace& other) = default;

StateSpace::StateSpace(StateSpace&& other) = default;

StateSpace& StateSpace::operator=(StateSpace&& other) = default;

StateSpace::~StateSpace() = default;

StateSpace& StateSpace::operator|=(const StateSpace& other) {
    m_states.insert(other.m_states.begin(), other.m_states.end());
    m_state_indices.insert(other.m_state_indices.begin(), other.m_state_indices.end());
    for (const auto& pair : other.m_forward_successor_state_indices) {
        m_forward_successor_state_indices[pair.first].insert(pair.second.begin(), pair.second.end());
    }
    m_goal_state_indices.insert(other.m_goal_state_indices.begin(), other.m_goal_state_indices.end());
    m_backward_successor_state_indices = compute_inverse_successor_state_indices(m_forward_successor_state_indices);
    return *this;
}

Distances StateSpace::compute_distances(const StateIndicesSet& state_indices, bool forward, bool stop_if_goal) const {
    Distances distances;
    std::deque<StateIndex> queue;
    for (StateIndex state : state_indices) {
        queue.push_back(state);
        distances[state] = 0;
    }
    const auto& successor_states = (forward) ? m_forward_successor_state_indices : m_backward_successor_state_indices;
    while (!queue.empty()) {
        int source = queue.front();
        queue.pop_front();
        if (stop_if_goal && m_goal_state_indices.count(source)) {
            continue;
        }
        const auto& targets = successor_states.find(source);
        if (targets != successor_states.end()) {
            for (int target : targets->second) {
                if (!distances.count(target)) {
                    distances[target] = distances[source] + 1;
                    queue.push_back(target);
                }
            }
        }
    }
    return distances;
}

void StateSpace::for_each_state_index(std::function<void(StateIndex state)>&& function) const {
    for (StateIndex state : m_state_indices) {
        function(state);
    }
}

void StateSpace::for_each_forward_successor_state_index(std::function<void(StateIndex)>&& function, StateIndex source) const {
    auto result = m_forward_successor_state_indices.find(source);
    if (result != m_forward_successor_state_indices.end()) {
        for (StateIndex successor : result->second) {
            function(successor);
        }
    }
}
void StateSpace::for_each_backward_successor_state_index(std::function<void(StateIndex)>&& function, StateIndex source) const {
    auto result = m_backward_successor_state_indices.find(source);
    if (result != m_backward_successor_state_indices.end()) {
        for (StateIndex successor : result->second) {
            function(successor);
        }
    }
}

bool StateSpace::is_goal(StateIndex state) const {
    return m_goal_state_indices.count(state);
}

bool StateSpace::is_nongoal(StateIndex state) const {
    return !is_goal(state);
}

bool StateSpace::is_trivially_solvable() const {
    return is_goal(m_initial_state_index);
}

const core::State& StateSpace::add_state(const core::State& state) {
    auto result = m_states.insert(state);
    if (!result.second) {
        m_state_indices.insert(state.get_index());
    }
    return *result.first;
}

void StateSpace::add_transition(StateIndex source, StateIndex target) {
    if (!m_state_indices.count(source)) {
        throw std::runtime_error("StateSpace::add_transition - source out of bound.");
    }
    if (!m_state_indices.count(target)) {
        throw std::runtime_error("StateSpace::add_transition - target out of bound");
    }
    m_forward_successor_state_indices[source].insert(target);
    m_backward_successor_state_indices[target].insert(source);
}

void StateSpace::print() const {
    std::cout << "Initial state index: " << m_initial_state_index << std::endl;
    std::cout << "States: " << std::to_string(get_num_states()) << std::endl;
    for (const auto& state : m_states) {
        std::cout << "    " << std::to_string(state.get_index()) << ":" << state.str() << std::endl;
    }
    std::cout << "Forward successors:" << std::endl;
    for_each_state_index(
        [this](StateIndex source){
            std::cout << "    " << source << ": ";
            for_each_forward_successor_state_index(
                [source](StateIndex target){
                    std::cout << target << " ";
                }, source);
            std::cout << std::endl;
        }
    );
    std::cout << "Backward successors:" << std::endl;
    for_each_state_index(
        [this](StateIndex source){
            std::cout << "    " << source << ": ";
            for_each_backward_successor_state_index(
                [source](StateIndex target){
                    std::cout << target << " ";
                }, source);
            std::cout << std::endl;
        }
    );
    std::cout << "Goal state indices: ";
    for (const auto goal_state : m_goal_state_indices) {
        std::cout << goal_state << " ";
    }
    std::cout << std::endl;
}

std::string StateSpace::to_dot(int verbosity_level) const {
    // 1. Precompute information for layout.
    auto goal_distance_information = compute_goal_distance_information();
    std::vector<StateIndices> layers;
    for (const auto& pair : goal_distance_information.get_goal_distances_ref()) {
        if (pair.second >= static_cast<int>(layers.size())) {
            layers.resize(pair.second + 1);
        }
        layers[pair.second].push_back(pair.first);
    }
    std::reverse(layers.begin(), layers.end());

    auto state_information = compute_state_information();
    std::stringstream result;
    // 2. Header
    result << "digraph {" << "\n"
           << "rankdir=\"LR\"" << "\n";
    // 3. Goal and nongoal states
    for (const auto& layer : layers) {
        for (auto state_index : layer) {
            result << "s" << state_index << "[";
            if (goal_distance_information.is_goal(state_index)) {
                result << "peripheries=2,";
            }
            result << "label=\"";
            if (verbosity_level >= 1) {
                result << state_information.get_state_ref(state_index).str();
            } else {
                result << state_index;
            }
            result << "\"]\n";
        }
    }
    // 4. initial state and dangling edge
    result << "Dangling [ label = \"\", style = invis ]\n"
           << "{ rank = same; Dangling }\n"
           << "Dangling -> s" << m_initial_state_index << "\n"
           << "{ rank = same; s" << m_initial_state_index << "}\n";
    // 5. Group states with same distane together
    for (int distance = 1; distance < static_cast<int>(layers.size()); ++distance) {
        // group states together
        result << "{ rank = same; ";
        auto& layer = layers[distance];
        for (auto state_index : layer) {
            result << "s" << state_index;
            if (state_index != layer.back()) {
                result << ",";
            }
        }
        result << "}\n";
    }
    // 6. Draw edges
    for (const auto& layer : layers) {
        result << "{\n";
        for (auto source_index : layer) {
            if (m_forward_successor_state_indices.count(source_index)) {
                for (auto target_index : m_forward_successor_state_indices.at(source_index)) {
                    result << "s" << source_index << "->" << "s" << target_index << "\n";
                }
            }
        }
        result << "}\n";
    }
    result << "}\n";
    return result.str();
}


GoalDistanceInformation StateSpace::compute_goal_distance_information() const {
    auto goal_distances = compute_distances(m_goal_state_indices, false, false);
    StateIndicesSet deadend_state_indices;
    for (StateIndex state : m_state_indices) {
        const auto& find = goal_distances.find(state);
        if (find == goal_distances.end()) {
            deadend_state_indices.insert(state);
        }
    }
    return GoalDistanceInformation(m_initial_state_index, std::move(goal_distances), std::move(deadend_state_indices));
}

StateInformation StateSpace::compute_state_information() const {
    StateMapping state_mapping;
    for (const auto& state : m_states) {
        state_mapping.emplace(state.get_index(), state);
    }
    return StateInformation(std::move(state_mapping));
}

void StateSpace::set_initial_state_index(StateIndex state_index) {
    m_initial_state_index = state_index;
}

void StateSpace::set_goal_state_indices(const StateIndicesSet& goal_states) {
    if (!std::all_of(goal_states.begin(), goal_states.end(), [this](StateIndex goal_state){ return m_state_indices.count(goal_state);})) {
        throw std::runtime_error("StateSpace::set_goal_state_indices - goal state out of bounds.");
    }
    m_goal_state_indices = goal_states;
}

const core::StatesSet& StateSpace::get_states_ref() const {
    return m_states;
}

const StateIndicesSet& StateSpace::get_state_indices_ref() const {
    return m_state_indices;
}

int StateSpace::get_num_states() const {
    return m_states.size();
}

StateIndex StateSpace::get_initial_state_index() const {
    return m_initial_state_index;
}

const AdjacencyList& StateSpace::get_forward_successor_state_indices_ref() const {
    return m_forward_successor_state_indices;
}

const AdjacencyList& StateSpace::get_backward_successor_state_indices_ref() const {
    return m_backward_successor_state_indices;
}

const StateIndicesSet& StateSpace::get_goal_state_indices_ref() const {
    return m_goal_state_indices;
}

const core::InstanceInfo& StateSpace::get_instance_info_ref() const {
    return *m_instance_info;
}

std::shared_ptr<const InstanceInfo> StateSpace::get_instance_info() const {
    return m_instance_info;
}

}
