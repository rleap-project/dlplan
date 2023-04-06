#include <cassert>
#include <iostream>
#include <vector>

#include "../../../include/dlplan/core.h"
#include "../../../include/dlplan/state_space.h"

using namespace dlplan::core;
using namespace dlplan::state_space;


int main() {
    // Generate and read state space files.
    auto state_space_2_1_0 = generate_state_space("domain.pddl", "instance_2_1_0.pddl", nullptr, 0);
    auto vocabulary_info = state_space_2_1_0.get_instance_info()->get_vocabulary_info();
    std::cout << "State space of instance_2_1_0:" << std::endl;
    state_space_2_1_0.print();
    // Generate and read state space files over same VocabularyInfo.
    auto state_space_2_2_0 = generate_state_space("domain.pddl", "instance_2_1_0.pddl", vocabulary_info, 1);
    std::cout << "State space of instance_2_2_0:" << std::endl;
    state_space_2_2_0.print();
    std::cout << std::endl;

    // Compute goal distances
    Distances goal_distance_info = state_space_2_1_0.compute_goal_distances();
    std::cout << "Goal distance information of instance_2_1_0:" << std::endl;
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

    // State information to obtain states by index
    std::cout << "State information of instance_2_1_0:" << std::endl;
    for (const auto& pair : state_space_2_1_0.get_states()) {
        std::cout << "state_index=" << pair.first << " atoms=" << pair.second.str() << std::endl;
    }

    // Compute forward distances from states with options forward=true, stop_if_goal=false
    Distances forward_distances = state_space_2_1_0.compute_distances({state_space_2_1_0.get_initial_state_index()}, true, false);
    std::cout << "Forward distances from initial state of instance_2_1_0:" << std::endl;
    for (const auto& pair : forward_distances) {
        std::cout << "state_index=" << pair.first << " distance=" << pair.second << std::endl;
    }
}
