#include "../../include/dlplan/state_space.h"

#include "generator.h"
#include "reader.h"

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
    StateMapping&& states,
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
        [this](const auto& pair){ return pair.second.get_instance_info() == this->get_instance_info(); })) {
        throw std::runtime_error("StateSpace::StateSpace - not all states come from the given InstanceInfo.");
    }
    if (!std::all_of(m_states.begin(), m_states.end(),
        [this](const auto& pair){ return pair.first == pair.second.get_index(); })) {
        throw std::runtime_error("StateSpace::StateSpace - invalid mapping from index to state.");
    }
    // assert goals
    if (!std::all_of(m_goal_state_indices.begin(), m_goal_state_indices.end(),
        [this](StateIndex goal_state){
            return m_states.count(goal_state);
        })) {
        throw std::runtime_error("StateSpace::StateSpace - goal state index out of bounds.");
    }
    // assert initial state
    if (!m_states.count(m_initial_state_index)) {
        throw std::runtime_error("StateSpace::StateSpace - initial state index out of bounds." + std::to_string(m_initial_state_index));
    }
    // assert forward successors
    if (!std::all_of(m_forward_successor_state_indices.begin(), m_forward_successor_state_indices.end(),
        [this](const auto& pair) {
            return m_states.count(pair.first);
        })) {
        throw std::runtime_error("StateSpace::StateSpace - source state index out of bounds.");
    }
    if (!std::all_of(m_forward_successor_state_indices.begin(), m_forward_successor_state_indices.end(),
        [this](const auto& pair){
            return std::all_of(pair.second.begin(), pair.second.end(),
            [this](StateIndex successor){
                return m_states.count(successor);
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
    for (const auto& pair : other.m_states) {
        StateIndex state_index = pair.first;
        if (generated_fragment.count(state_index)) {
            m_states.insert(pair);
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

Distances StateSpace::compute_goal_distances() const {
    return compute_distances(m_goal_state_indices, false, false);
}

void StateSpace::for_each_state(std::function<void(const State& state)>&& function) const {
    for (const auto& state : m_states) {
        function(state.second);
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

void StateSpace::print() const {
    std::cout << "Initial state index: " << m_initial_state_index << std::endl;
    std::cout << "States: " << std::to_string(m_states.size()) << std::endl;
    for (const auto& pair : m_states) {
        std::cout << "    " << std::to_string(pair.first) << ":" << pair.second.str() << std::endl;
    }
    std::cout << "Forward successors:" << std::endl;
    for_each_state(
        [this](const auto& source){
            std::cout << "    " << source.get_index() << ": ";
            for_each_forward_successor_state_index(
                [](StateIndex target){
                    std::cout << target << " ";
                }, source.get_index());
            std::cout << std::endl;
        }
    );
    std::cout << "Backward successors:" << std::endl;
    for_each_state(
        [this](const auto& source){
            std::cout << "    " << source.get_index() << ": ";
            for_each_backward_successor_state_index(
                [](StateIndex target){
                    std::cout << target << " ";
                }, source.get_index());
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
    auto goal_distances = compute_goal_distances();
    std::vector<StateIndices> layers;
    for (const auto& pair : goal_distances) {
        if (pair.second >= static_cast<int>(layers.size())) {
            layers.resize(pair.second + 1);
        }
        std::cout << pair.first << " " << pair.second << std::endl;
        layers[pair.second].push_back(pair.first);
    }
    // add deadends in next layer
    std::unordered_set<int> all_state_indices;
    std::for_each(m_states.begin(), m_states.end(), [&](const auto& pair){ all_state_indices.insert(pair.first); });
    std::unordered_set<int> added_deadends;
    for (int i = 1; i < layers.size(); ++i) {
        for (const int s_idx : layers[i]) {
            if (m_forward_successor_state_indices.count(s_idx) > 0) {
                for (const int s_prime_idx : m_forward_successor_state_indices.at(s_idx)) {
                    if (!goal_distances.count(s_prime_idx) && added_deadends.count(s_prime_idx) == 0) {
                        layers[i-1].push_back(s_prime_idx);
                        added_deadends.insert(s_prime_idx);
                    }
                }
            }
        }
    }
    // print in reverse direction
    std::reverse(layers.begin(), layers.end());

    std::stringstream result;
    // 2. Header
    result << "digraph {" << "\n"
           << "rankdir=\"LR\"" << "\n";
    // 3. Goal and nongoal states
    for (const auto& layer : layers) {
        for (auto state_index : layer) {
            result << "s" << state_index << "[";
            if (m_goal_state_indices.count(state_index)) {
                result << "peripheries=2,";
            }
            result << "label=\"";
            if (verbosity_level >= 1) {
                result << m_states.at(state_index).str();
            } else {
                result << state_index;
            }
            result << "\"]\n";
        }
    }
    // 4. initial state and dangling edge
    if (all_state_indices.count(m_initial_state_index)) {
        result << "Dangling [ label = \"\", style = invis ]\n"
            << "{ rank = same; Dangling }\n"
            << "Dangling -> s" << m_initial_state_index << "\n"
            << "{ rank = same; s" << m_initial_state_index << "}\n";
    }
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


void StateSpace::set_initial_state_index(StateIndex state_index) {
    m_initial_state_index = state_index;
}

void StateSpace::set_goal_state_indices(const StateIndicesSet& goal_states) {
    m_goal_state_indices = goal_states;
}

const StateMapping& StateSpace::get_states() const {
    return m_states;
}

StateIndex StateSpace::get_initial_state_index() const {
    return m_initial_state_index;
}

const AdjacencyList& StateSpace::get_forward_successor_state_indices() const {
    return m_forward_successor_state_indices;
}

const AdjacencyList& StateSpace::get_backward_successor_state_indices() const {
    return m_backward_successor_state_indices;
}

const StateIndicesSet& StateSpace::get_goal_state_indices() const {
    return m_goal_state_indices;
}

std::shared_ptr<const InstanceInfo> StateSpace::get_instance_info() const {
    return m_instance_info;
}

StateSpace generate_state_space(
    const std::string& domain_file,
    const std::string& instance_file,
    std::shared_ptr<const core::VocabularyInfo> vocabulary_info,
    int index) {
    generator::generate_state_space_files(domain_file, instance_file);
    return reader::read(vocabulary_info, index);
}

}
