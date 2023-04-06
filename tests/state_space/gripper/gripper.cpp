#include <gtest/gtest.h>

#include "../include/dlplan/state_space.h"

using namespace dlplan::core;
using namespace dlplan::state_space;


TEST(DLPTests, StateSpaceGripperTest) {
    auto state_space = generate_state_space("domain.pddl", "p-1-0.pddl");
    auto goal_distances = state_space.compute_goal_distances();
    state_space.print();
    std::cout << state_space.to_dot(0) << std::endl;
    EXPECT_EQ(goal_distances.count(state_space.get_initial_state_index()), 1);
}
