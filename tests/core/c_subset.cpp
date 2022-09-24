#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, ConceptSubset) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("role_1", 2);
    Predicate p1 = vocabulary->add_predicate("role_2", 2);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    // Add state atoms
    Atom a0 = instance->add_atom("role_1", {"A", "B"});
    Atom a1 = instance->add_atom("role_1", {"B", "C"});

    Atom a2 = instance->add_atom("role_2", {"A", "B"});
    Atom a3 = instance->add_atom("role_2", {"A", "C"});
    Atom a4 = instance->add_atom("role_2", {"B", "D"});
    Atom a5 = instance->add_atom("role_2", {"C", "D"});

    State state(instance, {a0, a1, a2, a3, a4, a5}, 0);

    SyntacticElementFactory factory(vocabulary);
    DenotationsCaches caches;

    Concept concept1 = factory.parse_concept("c_subset(r_primitive(role_1,0,1),r_primitive(role_2,0,1))");
    EXPECT_EQ(concept1.evaluate(state).to_sorted_vector(), Index_Vec({0, 2, 3}));
    EXPECT_EQ(concept1.evaluate(state, caches)->to_sorted_vector(), Index_Vec({0, 2, 3}));
    EXPECT_EQ(concept1.evaluate({state}, caches)->to_sorted_vector(), Index_Vec({0, 2, 3}));

    Concept concept2 = factory.parse_concept("c_subset(r_primitive(role_2,0,1),r_primitive(role_1,0,1))");
    EXPECT_EQ(concept2.evaluate(state).to_sorted_vector(), Index_Vec({3}));
    EXPECT_EQ(concept2.evaluate(state, caches)->to_sorted_vector(), Index_Vec({3}));
    EXPECT_EQ(concept2.evaluate({state}, caches)->to_sorted_vector(), Index_Vec({3}));
}
