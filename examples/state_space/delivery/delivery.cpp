#include <cassert>
#include <iostream>
#include <vector>

#include "../../../include/dlplan/core.h"
#include "../../../include/dlplan/state_space.h"

using namespace dlplan::core;
using namespace dlplan::state_space;


int main() {
    /* Note: workspace must the be directory containing the binary in the build directory
       because StateSpaceGenerator executes ./fast-downward.py
       from current workspace and outputs state space data files into current workspace.
       The motivation for this is that when we run experiments on a grid
       we need to use separate output directories to not overwrite
       state space data files of experiments running in other threads.
       Hence, it is less error prone to create a symbolic link to the planner
       and setting the workspace accordingly instead of passing an absolute path to the planner.
    */
    // Generate and read state space files.
    StateSpaceGenerator().generate_state_space("domain.pddl", "instance_2_1_0.pddl");
    auto state_space_2_1_0 = StateSpaceReader().read(nullptr, 0);
    auto vocabulary_info = state_space_2_1_0.get_instance_info_ref().get_vocabulary_info();
    std::cout << "State space of instance_2_1_0:" << std::endl;
    state_space_2_1_0.print();
    // Generate and read state space files over same VocabularyInfo.
    StateSpaceGenerator().generate_state_space("domain.pddl", "instance_2_1_0.pddl");
    auto state_space_2_2_0 = StateSpaceReader().read(vocabulary_info, 1);
    std::cout << "State space of instance_2_2_0:" << std::endl;
    state_space_2_2_0.print();
    std::cout << std::endl;

    // Compute goal distances
    GoalDistanceInformation goal_distance_info = state_space_2_1_0.compute_goal_distance_information();
    std::cout << "Goal distance information of instance_2_1_0:" << std::endl;
    std::cout << "Goal distances:" << std::endl;
    for (const auto& pair : goal_distance_info.get_goal_distances_ref()) {
        std::cout << "state_index=" << pair.first << " distance=" << pair.second << std::endl;
    }
    std::cout << "Deadends:" << std::endl;
    for (const int deadend : goal_distance_info.get_deadend_state_indices_ref()) {
        std::cout << deadend << " ";
    }
    std::cout << std::endl << std::endl;

    // State information to obtain states by index
    StateInformation state_information = state_space_2_1_0.compute_state_information();
    std::cout << "State information of instance_2_1_0:" << std::endl;
    for (auto state_index : state_space_2_1_0.get_state_indices_ref()) {
        const auto& state = state_information.get_state_ref(state_index);
        std::cout << "state_index=" << state.get_index() << " atoms=" << state.str() << std::endl;
    }

    // Compute forward distances from states with options forward=true, stop_if_goal=false
    Distances forward_distances = state_space_2_1_0.compute_distances({state_space_2_1_0.get_initial_state_index()}, true, false);
    std::cout << "Forward distances from initial state of instance_2_1_0:" << std::endl;
    for (const auto& pair : forward_distances) {
        std::cout << "state_index=" << pair.first << " distance=" << pair.second << std::endl;
    }
}
