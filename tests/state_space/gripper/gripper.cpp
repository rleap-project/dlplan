#include <gtest/gtest.h>

#include "../include/dlplan/state_space.h"

using namespace dlplan::core;
using namespace dlplan::state_space;


TEST(DLPTests, StateSpaceGripperTest) {
    StateSpaceGenerator().generate_state_space("domain.pddl", "p-1-0.pddl");
    auto state_space = StateSpaceReader().read();
    auto distance_information = state_space.compute_goal_distance_information();
    state_space.print();
    std::cout << state_space.to_dot(0) << std::endl;
    EXPECT_EQ(distance_information.is_solvable(), true);
}
