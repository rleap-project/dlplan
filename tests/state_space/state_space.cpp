#include <gtest/gtest.h>

#include "../include/dlplan/state_space.h"

using namespace dlplan::core;
using namespace dlplan::state_space;


TEST(DLPTests, StateSpaceTest) {
    auto state_space = StateSpaceGenerator().generate_state_space("domain.pddl", "instance.pddl");
    state_space.print();
    ASSERT_EQ(state_space.is_solvable(), true);
}
