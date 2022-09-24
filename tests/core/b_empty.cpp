#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, BooleanEmpty) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("concept_0", 1);
    Predicate p1 = vocabulary->add_predicate("concept_1", 1);
    Predicate p2 = vocabulary->add_predicate("role_0", 2);
    Predicate p3 = vocabulary->add_predicate("role_1", 2);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    // Add state atoms
    Atom a0 = instance->add_atom("concept_0", {"A"});
    Atom a1 = instance->add_atom("role_0", {"A", "B"});

    State state(instance, {a0, a1}, 0);

    SyntacticElementFactory factory(vocabulary);
    DenotationsCaches caches;

    Boolean boolean1 = factory.parse_boolean("b_empty(c_primitive(concept_0,0))");
    EXPECT_EQ(boolean1.evaluate(state), false);
    EXPECT_EQ(boolean1.evaluate(state, caches), false);
    EXPECT_EQ(boolean1.evaluate({state}, caches), false);

    Boolean boolean2 = factory.parse_boolean("b_empty(c_primitive(concept_1,0))");
    EXPECT_EQ(boolean2.evaluate(state), true);
    EXPECT_EQ(boolean2.evaluate(state, caches), true);
    EXPECT_EQ(boolean2.evaluate({state}, caches), true);

    Boolean boolean3 = factory.parse_boolean("b_empty(r_primitive(role_0,0,1))");
    EXPECT_EQ(boolean3.evaluate(state), false);
    EXPECT_EQ(boolean3.evaluate(state, caches), false);
    EXPECT_EQ(boolean3.evaluate({state}, caches), false);

    Boolean boolean4 = factory.parse_boolean("b_empty(r_primitive(role_1,0,1))");
    EXPECT_EQ(boolean4.evaluate(state), true);
    EXPECT_EQ(boolean4.evaluate(state, caches), true);
    EXPECT_EQ(boolean4.evaluate({state}, caches), true);
}
