#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, ConceptAnd) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("concept_1", 1);
    Predicate p1 = vocabulary->add_predicate("concept_2", 1);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    // Add state atoms
    Atom a0 = instance->add_atom("concept_1", {"A"});
    Atom a1 = instance->add_atom("concept_1", {"B"});

    Atom a2 = instance->add_atom("concept_2", {"B"});
    Atom a3 = instance->add_atom("concept_2", {"C"});

    State state(instance, {a0, a1, a2, a3}, 0);

    SyntacticElementFactory factory(vocabulary);
    DenotationsCaches caches;

    Concept concept = factory.parse_concept("c_and(c_primitive(concept_1,0),c_primitive(concept_2,0))");

    EXPECT_EQ(concept.evaluate(state).to_sorted_vector(), Index_Vec({1}));
    EXPECT_EQ(concept.evaluate(state, caches)->to_sorted_vector(), Index_Vec({1}));
    EXPECT_EQ(concept.evaluate({state}, caches)->to_sorted_vector(), Index_Vec({1}));
}
