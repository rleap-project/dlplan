#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, NumericalCount) {
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

    Numerical numerical1 = factory.parse_numerical("n_count(c_primitive(concept_0,0))");
    EXPECT_EQ(numerical1.evaluate(state), 1);
    EXPECT_EQ(numerical1.evaluate(state, caches), 1);
    EXPECT_EQ(numerical1.evaluate({state}, caches), 1);

    Numerical numerical2 = factory.parse_numerical("n_count(c_primitive(concept_1,0))");
    EXPECT_EQ(numerical2.evaluate(state), 0);
    EXPECT_EQ(numerical2.evaluate(state, caches), 0);
    EXPECT_EQ(numerical2.evaluate({state}, caches), 0);

    Numerical numerical3 = factory.parse_numerical("n_count(r_primitive(role_0,0,1))");
    EXPECT_EQ(numerical3.evaluate(state), 1);
    EXPECT_EQ(numerical3.evaluate(state, caches), 1);
    EXPECT_EQ(numerical3.evaluate({state}, caches), 1);

    Numerical numerical4 = factory.parse_numerical("n_count(r_primitive(role_1,0,1))");
    EXPECT_EQ(numerical4.evaluate(state), 0);
    EXPECT_EQ(numerical4.evaluate(state, caches), 0);
    EXPECT_EQ(numerical4.evaluate({state}, caches), 0);
}
