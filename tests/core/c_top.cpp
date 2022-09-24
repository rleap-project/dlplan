#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, ConceptTop) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("concept", 1);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    // Add state atoms
    Atom a0 = instance->add_atom("concept", {"A"});
    Atom a1 = instance->add_atom("concept", {"B"});

    State state(instance, {a0, a1}, 0);

    SyntacticElementFactory factory(vocabulary);
    DenotationsCaches caches;

    Concept concept = factory.parse_concept("c_top");
    EXPECT_EQ(concept.evaluate(state).to_sorted_vector(), Index_Vec({0, 1}));
    EXPECT_EQ(concept.evaluate(state, caches)->to_sorted_vector(), Index_Vec({0, 1}));
    EXPECT_EQ(concept.evaluate({state}, caches)->to_sorted_vector(), Index_Vec({0, 1}));
}
