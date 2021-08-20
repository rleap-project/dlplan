#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


// Test the transitive closure
TEST(DLPTests, ConceptAnd) {
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

    State state(instance, {a0, a1, a2, a3});

    SyntacticElementFactory factory(vocabulary);

    Concept concept = factory.parse_concept("c_and(concept_1(0),concept_2(0))");
    EXPECT_EQ(concept.evaluate(state), dlplan::core::ConceptDenotation({1}));
}
