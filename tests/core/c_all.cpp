#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, ConceptAll) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("role", 2);
    Predicate p1 = vocabulary->add_predicate("concept", 1);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    // Add state atoms
    Atom a0 = instance->add_atom("role", {"A", "B"});
    Atom a1 = instance->add_atom("role", {"A", "C"});
    Atom a2 = instance->add_atom("role", {"B", "C"});
    Atom a3 = instance->add_atom("role", {"B", "D"});
    Atom a4 = instance->add_atom("role", {"E", "D"});

    Atom a5 = instance->add_atom("concept", {"B"});
    Atom a6 = instance->add_atom("concept", {"C"});

    State state(instance, {a0, a1, a2, a3, a4, a5, a6}, 0);

    SyntacticElementFactory factory(vocabulary);

    Concept concept = factory.parse_concept("c_all(r_primitive(role,0,1),c_primitive(concept,0))");
    EXPECT_EQ(concept.evaluate(state).to_sorted_vector(), Index_Vec({0, 2, 3}));

    DenotationsCaches caches;
    EXPECT_EQ(concept.evaluate(state), *concept.evaluate(state, caches));
}

TEST(DLPTests, ConceptAll2) {
    // Example for the spanner domain
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("at", 2);
    Predicate p1 = vocabulary->add_predicate("man", 1);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    // Add state atoms
    Atom a0 = instance->add_atom("at", {"spanner_1", "location_1"});
    Atom a1 = instance->add_atom("at", {"spanner_2", "location_2"});
    Atom a2 = instance->add_atom("at", {"bob", "location_1"});
    Atom a3 = instance->add_atom("man", {"bob"});

    State state_1(instance, {a0, a1, a2, a3}, 0);  // bob and spanner_1 at location_1
    State state_2(instance, {a1, a2, a3}, 1);  // only bob at location_1

    SyntacticElementFactory factory(vocabulary);
    DenotationsCaches caches;

    Concept concept = factory.parse_concept("c_all(r_primitive(at,1,0),c_primitive(man,0))");
    EXPECT_EQ(concept.evaluate(state_1).to_sorted_vector(), Index_Vec({0, 2, 4}));
    EXPECT_EQ(concept.evaluate(state_1, caches)->to_sorted_vector(), Index_Vec({0, 2, 4}));

    EXPECT_EQ(concept.evaluate(state_2).to_sorted_vector(), Index_Vec({0, 1, 2, 4}));
    EXPECT_EQ(concept.evaluate(state_2, caches)->to_sorted_vector(), Index_Vec({0, 1, 2, 4}));
}
