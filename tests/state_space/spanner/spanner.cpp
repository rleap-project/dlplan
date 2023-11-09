#include <gtest/gtest.h>

#include "../../../include/dlplan/state_space.h"

using namespace dlplan::core;
using namespace dlplan::state_space;


namespace dlplan::tests::state_space {

TEST(DLPTests, StateSpaceSpannerTest) {
    auto result = generate_state_space("domain.pddl", "problem.pddl");
    const auto& state_space = *result.state_space;
    auto goal_distances = state_space.compute_goal_distances();
    std::cout << state_space.str() << std::endl;
    std::cout << state_space.to_dot(1) << std::endl;
    EXPECT_EQ(goal_distances.count(state_space.get_initial_state_index()), 1);
}

}
