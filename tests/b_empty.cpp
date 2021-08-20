#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;



// Test the transitive closure
TEST(DLPTests, BooleanEmpty) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("predicate_0", 1);
    Predicate p1 = vocabulary->add_predicate("predicate_1", 1);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary);
    // Add state atoms
    Atom a0 = instance->add_atom("predicate_0", {"A"});

    State state(instance, {a0});

    SyntacticElementFactory factory(vocabulary);

    Boolean boolean1 = factory.parse_boolean("b_empty(predicate_0(0))");
    EXPECT_EQ(boolean1.evaluate(state), false);

    Boolean boolean2 = factory.parse_boolean("b_empty(predicate_1(0))");
    EXPECT_EQ(boolean2.evaluate(state), true);
}
