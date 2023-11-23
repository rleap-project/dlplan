#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, NumericalCount) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("concept_0", 1);
    auto predicate_1 = vocabulary->add_predicate("concept_1", 1);
    auto predicate_2 = vocabulary->add_predicate("role_0", 2);
    auto predicate_3 = vocabulary->add_predicate("role_1", 2);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0 = instance->add_atom("concept_0", {"A"});
    auto atom_1 = instance->add_atom("role_0", {"A", "B"});

    State state_0(0, instance, {atom_0, atom_1});

    SyntacticElementFactory factory(vocabulary);

    auto numerical_0 = factory.parse_numerical("n_count(c_primitive(concept_0,0))");
    EXPECT_EQ(numerical_0->evaluate(state_0), 1);

    auto numerical_1 = factory.parse_numerical("n_count(c_primitive(concept_1,0))");
    EXPECT_EQ(numerical_1->evaluate(state_0), 0);

    auto numerical_2 = factory.parse_numerical("n_count(r_primitive(role_0,0,1))");
    EXPECT_EQ(numerical_2->evaluate(state_0), 1);

    auto numerical_3 = factory.parse_numerical("n_count(r_primitive(role_1,0,1))");
    EXPECT_EQ(numerical_3->evaluate(state_0), 0);
}

}
