#include <gtest/gtest.h>

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::core::tests {

TEST(DLPTests, RoleDenotationComputeRepr) {
    int num_objects = 4;
    RoleDenotation denotation(num_objects);
    denotation.insert({1,2});
    denotation.insert({0,1});
    EXPECT_EQ(denotation.str(), "RoleDenotation(num_objects=4, pairs_of_object_indices=[<0,1>, <1,2>])");
}

}
