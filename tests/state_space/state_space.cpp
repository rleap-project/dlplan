#include <gtest/gtest.h>

#include "../include/dlplan/state_space.h"

using namespace dlplan::core;
using namespace dlplan::state_space;


TEST(DLPTests, StateSpaceTest) {
    StateSpaceGenerator().generate_state_space("domain.pddl", "p-1-0.pddl");
}
