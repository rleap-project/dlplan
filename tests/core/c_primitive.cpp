#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, ConceptPrimitive) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("predicate", 3);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    // Add state atoms
    Atom a0 = instance->add_atom("predicate", {"A", "B", "C"});
    Atom a1 = instance->add_atom("predicate", {"D", "E", "F"});

    State state(instance, {a0, a1}, 0);

    SyntacticElementFactory factory(vocabulary);
    DenotationsCaches caches;

    Concept concept1 = factory.parse_concept("c_primitive(predicate,0)");
    EXPECT_EQ(concept1.evaluate(state).to_sorted_vector(), Index_Vec({0, 3}));
    EXPECT_EQ(concept1.evaluate(state, caches)->to_sorted_vector(), Index_Vec({0, 3}));
    EXPECT_EQ(concept1.evaluate({state}, caches)->to_sorted_vector(), Index_Vec({0, 3}));

    Concept concept2 = factory.parse_concept("c_primitive(predicate,1)");
    EXPECT_EQ(concept2.evaluate(state).to_sorted_vector(), Index_Vec({1, 4}));
    EXPECT_EQ(concept2.evaluate(state, caches)->to_sorted_vector(), Index_Vec({1, 4}));
    EXPECT_EQ(concept2.evaluate({state}, caches)->to_sorted_vector(), Index_Vec({1, 4}));

    Concept concept3 = factory.parse_concept("c_primitive(predicate,2)");
    EXPECT_EQ(concept3.evaluate(state).to_sorted_vector(), Index_Vec({2, 5}));
    EXPECT_EQ(concept3.evaluate(state, caches)->to_sorted_vector(), Index_Vec({2, 5}));
    EXPECT_EQ(concept3.evaluate({state}, caches)->to_sorted_vector(), Index_Vec({2, 5}));
}
