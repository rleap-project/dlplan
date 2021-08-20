#include <limits>

#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;



// Test the transitive closure
TEST(DLPTests, NumericalSumRoleDistance) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("conn", 2);
    Predicate p1 = vocabulary->add_predicate("start", 2);
    Predicate p2 = vocabulary->add_predicate("end", 2);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary);
    // Add state atoms
    Atom a0 = instance->add_atom("conn", {"A", "B"});
    Atom a1 = instance->add_atom("conn", {"B", "C"});

    Atom a2 = instance->add_atom("start", {"X", "A"});
    Atom a3 = instance->add_atom("end", {"Y", "C"});
    Atom a4 = instance->add_atom("end", {"X", "C"});

    State state(instance, {a0, a1, a2, a3, a4});

    SyntacticElementFactory factory(vocabulary);

    Numerical numerical = factory.parse_numerical("n_sum_role_distance(start(0,1), conn(0,1), end(0,1))");
    EXPECT_EQ(numerical.evaluate(state), 2);
}
