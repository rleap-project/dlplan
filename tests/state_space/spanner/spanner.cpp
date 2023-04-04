#include <gtest/gtest.h>

#include "../include/dlplan/state_space.h"

using namespace dlplan::core;
using namespace dlplan::state_space;


TEST(DLPTests, StateSpannerGripperTest) {
    auto state_space = generate_state_space("domain.pddl", "problem.pddl");
    state_space.set_initial_state_index(-1);
    auto distance_information = state_space.compute_goal_distance_information();
    state_space.print();
    std::cout << state_space.to_dot(1) << std::endl;
    EXPECT_EQ(distance_information.is_alive(state_space.get_initial_state_index()), true);
}
