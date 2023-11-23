#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

#include <limits>

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, NumericalConceptDistance) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("conn", 2);
    auto predicate_1 = vocabulary->add_predicate("start", 1);
    auto predicate_2 = vocabulary->add_predicate("end", 1);
    auto predicate_3 = vocabulary->add_predicate("start2", 1);
    auto predicate_4 = vocabulary->add_predicate("end2", 1);
    auto predicate_5 = vocabulary->add_predicate("start3", 1);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0 = instance->add_atom("conn", {"A", "B"});
    auto atom_1 = instance->add_atom("conn", {"B", "C"});
    auto atom_2 = instance->add_atom("conn", {"C", "A"});
    auto atom_3 = instance->add_atom("conn", {"D", "E"});
    auto atom_4 = instance->add_atom("conn", {"E", "A"});
    auto atom_5 = instance->add_atom("start", {"D"});
    auto atom_6 = instance->add_atom("end", {"A"});
    auto atom_7 = instance->add_atom("end", {"B"});
    auto atom_8 = instance->add_atom("start2", {"B"});
    auto atom_9 = instance->add_atom("end2", {"E"});

    State state_0(0, instance, {atom_0, atom_1, atom_2, atom_3, atom_4, atom_5, atom_6, atom_7, atom_8, atom_9});

    SyntacticElementFactory factory(vocabulary);

    auto numerical_0 = factory.parse_numerical("n_concept_distance(c_primitive(start,0),r_primitive(conn,0,1),c_primitive(end,0))");
    EXPECT_EQ(numerical_0->evaluate(state_0), 2);

    auto numerical_1 = factory.parse_numerical("n_concept_distance(c_primitive(start2,0),r_primitive(conn,0,1),c_primitive(end2,0))");
    EXPECT_EQ(numerical_1->evaluate(state_0), std::numeric_limits<int>::max());

    // SumConceptDistance with no sources is defined as 0
    auto numerical_2 = factory.parse_numerical("n_concept_distance(c_primitive(start3,0),r_primitive(conn,0,1),c_primitive(end2,0))");
    EXPECT_EQ(numerical_2->evaluate(state_0), std::numeric_limits<int>::max());
}

}
