#include <limits>

#include <gtest/gtest.h>

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, MultiInstance) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("conn", 2);
    auto predicate_1 = vocabulary->add_predicate("start", 1);
    auto predicate_2 = vocabulary->add_predicate("end", 1);

    // Instance with graph consisting of nodes A,B,C,D and edges A->C,C->B,B->D
    auto instance_0 = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0_0 = instance_0->add_static_atom("conn", {"A", "C"});
    auto atom_0_1 = instance_0->add_static_atom("conn", {"C", "B"});
    auto atom_0_2 = instance_0->add_static_atom("conn", {"B", "D"});
    auto atom_0_3 = instance_0->add_atom("start", {"A"});
    auto atom_0_4 = instance_0->add_atom("end", {"D"});
    State state_0_0(0, instance_0, {atom_0_3, atom_0_4});

    // Instance with graph consisting of nodes A,B,C,D and edges A->B,B->C,C->D
    auto instance_1 = std::make_shared<InstanceInfo>(1, vocabulary);
    auto atom_1_0 = instance_1->add_static_atom("conn", {"A", "B"});
    auto atom_1_1 = instance_1->add_static_atom("conn", {"B", "C"});
    auto atom_1_2 = instance_1->add_static_atom("conn", {"C", "D"});
    auto atom_1_3 = instance_1->add_atom("start", {"A"});
    auto atom_1_4 = instance_1->add_atom("end", {"D"});
    State state_1_0(0, instance_1, {atom_1_3, atom_1_4});

    // Instance where all atoms of instance1 and instance2 are added
    auto instance = std::make_shared<InstanceInfo>(2, vocabulary);
    auto atom_0 = instance->add_static_atom("conn", {"A", "C"});
    auto atom_1 = instance->add_static_atom("conn", {"C", "B"});
    auto atom_2 = instance->add_static_atom("conn", {"B", "D"});
    auto atom_3 = instance->add_static_atom("conn", {"A", "B"});
    auto atom_4 = instance->add_static_atom("conn", {"B", "C"});
    auto atom_5 = instance->add_static_atom("conn", {"C", "D"});
    auto atom_6 = instance->add_atom("start", {"A"});
    auto atom_7 = instance->add_atom("end", {"D"});
    State state_0(0, instance, {atom_6, atom_7});

    SyntacticElementFactory factory(vocabulary);

    auto numerical = factory.parse_numerical("n_concept_distance(c_primitive(start,0), r_primitive(conn,0,1), c_primitive(end,0))");
    EXPECT_EQ(numerical->evaluate(state_0_0), 3);
    EXPECT_EQ(numerical->evaluate(state_1_0), 3);
    // Shortest distance in union of graphs reduces to 2
    EXPECT_EQ(numerical->evaluate(state_0), 2);
}

}
