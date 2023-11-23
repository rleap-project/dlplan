#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, ConceptAll) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("role", 2);
    auto predicate_1 = vocabulary->add_predicate("concept", 1);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0 = instance->add_atom("role", {"A", "B"});
    auto atom_1 = instance->add_atom("role", {"A", "C"});
    auto atom_2 = instance->add_atom("role", {"B", "C"});
    auto atom_3 = instance->add_atom("role", {"B", "D"});
    auto atom_4 = instance->add_atom("role", {"E", "D"});
    auto atom_5 = instance->add_atom("concept", {"B"});
    auto atom_6 = instance->add_atom("concept", {"C"});

    State state_0(0, instance, {atom_0, atom_1, atom_2, atom_3, atom_4, atom_5, atom_6});

    SyntacticElementFactory factory(vocabulary);

    auto concept_ = factory.parse_concept("c_all(r_primitive(role,0,1),c_primitive(concept,0))");
    EXPECT_EQ(concept_->evaluate(state_0), create_concept_denotation(*instance, {"A", "C", "D"}));
}

TEST(DLPTests, ConceptAll2) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("at", 2);
    auto predicate_1 = vocabulary->add_predicate("man", 1);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0 = instance->add_atom("at", {"spanner_1", "location_1"});
    auto atom_1 = instance->add_atom("at", {"spanner_2", "location_2"});
    auto atom_2 = instance->add_atom("at", {"bob", "location_1"});
    auto atom_3 = instance->add_atom("man", {"bob"});

    State state_0(0, instance, {atom_0, atom_1, atom_2, atom_3});  // bob and spanner_1 at location_1
    State state_1(1, instance, {atom_1, atom_2, atom_3});  // only bob at location_1

    SyntacticElementFactory factory(vocabulary);

    auto concept_ = factory.parse_concept("c_all(r_primitive(at,1,0),c_primitive(man,0))");
    EXPECT_EQ(concept_->evaluate(state_0), create_concept_denotation(*instance, {"spanner_1", "spanner_2", "bob"}));
    EXPECT_EQ(concept_->evaluate(state_1), create_concept_denotation(*instance, {"spanner_1", "location_1", "spanner_2", "bob"}));
}

}
