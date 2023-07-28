/// Example illustrating the state_space component.

#include "../../../include/dlplan/core.h"
#include "../../../include/dlplan/state_space.h"

#include <cassert>
#include <iostream>
#include <vector>

using namespace dlplan::state_space;


/// @brief Generate state spaces over the same vocabulary and compute distances.
int main() {
    // Generate and read state space files.
    auto result = generate_state_space("domain.pddl", "instance_2_1_0.pddl", nullptr, 0);
    const auto& state_space_2_1_0 = *result.state_space;
    auto vocabulary_info = state_space_2_1_0.get_instance_info()->get_vocabulary_info();
    std::cout << "State space of instance_2_1_0:" << std::endl;
    std::cout << state_space_2_1_0.str() << std::endl;

    // Generate and read state space files over same VocabularyInfo.
    auto result2 = generate_state_space("domain.pddl", "instance_2_1_0.pddl", vocabulary_info, 1);
    const auto& state_space_2_2_0 = *result2.state_space;
    std::cout << "State space of instance_2_2_0:" << std::endl;
    std::cout << state_space_2_2_0.str() << std::endl;
    std::cout << std::endl;

    // Compute goal distances and deadends
    Distances goal_distance_info = state_space_2_1_0.compute_goal_distances();
    std::cout << "Goal distances:" << std::endl;
    for (const auto& pair : goal_distance_info) {
        std::cout << "state_index=" << pair.first << " distance=" << pair.second << std::endl;
    }
    std::cout << "Deadends:" << std::endl;
    for (const auto& pair : state_space_2_1_0.get_states()) {
        if (!goal_distance_info.count(pair.first)) {
            std::cout << pair.first << " ";
        }
    }
    std::cout << std::endl << std::endl;

    // Compute forward distances from states with options forward=true, stop_if_goal=false
    Distances forward_distances = state_space_2_1_0.compute_distances({state_space_2_1_0.get_initial_state_index()}, true, false);
    std::cout << "Forward distances from initial state of instance_2_1_0:" << std::endl;
    for (const auto& pair : forward_distances) {
        std::cout << "state_index=" << pair.first << " distance=" << pair.second << std::endl;
    }
}
