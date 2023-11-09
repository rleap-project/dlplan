#include <gtest/gtest.h>

#include "../../../../include/dlplan/state_space.h"

using namespace dlplan::core;
using namespace dlplan::state_space;


namespace dlplan::tests::state_space {

TEST(DLPTests, StateSpaceGripperIncompleteTest) {
    auto result = generate_state_space("domain.pddl", "p-1-0.pddl", nullptr, -1, 0, 0);
    EXPECT_EQ(result.exit_code, GeneratorExitCode::INCOMPLETE);
}

}
