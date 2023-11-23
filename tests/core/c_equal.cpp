#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, ConceptEqual) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("role_1", 2);
    auto predicate_1 = vocabulary->add_predicate("role_2", 2);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0 = instance->add_atom("role_1", {"A", "B"});
    auto atom_1 = instance->add_atom("role_1", {"B", "C"});
    auto atom_2 = instance->add_atom("role_1", {"C", "Table"});
    auto atom_3 = instance->add_atom("role_2", {"A", "B"});
    auto atom_4 = instance->add_atom("role_2", {"B", "Table"});
    auto atom_5 = instance->add_atom("role_2", {"C", "Table"});

    State state_0(0, instance, {atom_0, atom_1, atom_2, atom_3, atom_4, atom_5});

    SyntacticElementFactory factory(vocabulary);

    auto concept1 = factory.parse_concept("c_equal(r_primitive(role_1,0,1),r_primitive(role_2,0,1))");
    EXPECT_EQ(concept1->evaluate(state_0), create_concept_denotation(*instance, {"A", "C", "Table"}));

    auto concept2 = factory.parse_concept("c_equal(r_primitive(role_2,0,1),r_primitive(role_1,0,1))");
    EXPECT_EQ(concept2->evaluate(state_0), create_concept_denotation(*instance, {"A", "C", "Table"}));
}

}
