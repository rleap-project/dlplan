#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, ConceptInclusion) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("concept_1", 1);
    Predicate p1 = vocabulary->add_predicate("concept_2", 1);

    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary);
    // Add state atoms
    Atom a0 = instance->add_atom("concept_1", {"A"});
    Atom a1 = instance->add_atom("concept_1", {"B"});

    Atom a2 = instance->add_atom("concept_2", {"B"});
    Atom a3 = instance->add_atom("concept_2", {"C"});

    State s1(instance, {a1, a2, a3});  // {B} subseteq {B, C}
    State s2(instance, {a0, a1, a2, a3});  // {A, B} not subseteq {B, C}

    SyntacticElementFactory factory(vocabulary);

    EvaluationCaches caches(instance);

    Boolean boolean = factory.parse_boolean("b_inclusion(c_primitive(concept_1,0),c_primitive(concept_2,0))");
    EXPECT_EQ(boolean.evaluate(s1, caches), true);
    caches.clear();
    EXPECT_EQ(boolean.evaluate(s2, caches), false);
    caches.clear();
}
