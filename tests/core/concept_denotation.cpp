#include <gtest/gtest.h>

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::core::tests {

TEST(DLPTests, ConceptDenotationComputeRepr) {
    int num_objects = 4;
    ConceptDenotation denotation(num_objects);
    denotation.insert(2);
    denotation.insert(0);
    EXPECT_EQ(denotation.str(), "ConceptDenotation(num_objects=4, object_indices=[0, 2])");
}

}
