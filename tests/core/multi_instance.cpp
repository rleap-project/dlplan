#include <limits>

#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, MultiInstance) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("conn", 2);
    Predicate p1 = vocabulary->add_predicate("start", 1);
    Predicate p2 = vocabulary->add_predicate("end", 1);

    // Instance with graph consisting of nodes A,B,C,D and edges A->C,C->B,B->D
    std::shared_ptr<InstanceInfo> instance1 = std::make_shared<InstanceInfo>(vocabulary, 0);
    Atom a1_0 = instance1->add_static_atom("conn", {"A", "C"});
    Atom a1_1 = instance1->add_static_atom("conn", {"C", "B"});
    Atom a1_2 = instance1->add_static_atom("conn", {"B", "D"});
    Atom a1_3 = instance1->add_atom("start", {"A"});
    Atom a1_4 = instance1->add_atom("end", {"D"});
    State state1(instance1, {a1_3, a1_4});

    // Instance with graph consisting of nodes A,B,C,D and edges A->B,B->C,C->D
    std::shared_ptr<InstanceInfo> instance2 = std::make_shared<InstanceInfo>(vocabulary, 1);
    Atom a2_0 = instance2->add_static_atom("conn", {"A", "B"});
    Atom a2_1 = instance2->add_static_atom("conn", {"B", "C"});
    Atom a2_2 = instance2->add_static_atom("conn", {"C", "D"});
    Atom a2_3 = instance2->add_atom("start", {"A"});
    Atom a2_4 = instance2->add_atom("end", {"D"});
    State state2(instance2, {a2_3, a2_4});

    // Instance where all atoms of instance1 and instance2 are added
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 2);
    Atom a0 = instance->add_static_atom("conn", {"A", "C"});
    Atom a1 = instance->add_static_atom("conn", {"C", "B"});
    Atom a2 = instance->add_static_atom("conn", {"B", "D"});
    Atom a3 = instance->add_static_atom("conn", {"A", "B"});
    Atom a4 = instance->add_static_atom("conn", {"B", "C"});
    Atom a5 = instance->add_static_atom("conn", {"C", "D"});
    Atom a6 = instance->add_atom("start", {"A"});
    Atom a7 = instance->add_atom("end", {"D"});
    State state(instance, {a6, a7});

    SyntacticElementFactory factory(vocabulary);

    Numerical numerical = factory.parse_numerical("n_concept_distance(c_primitive(start,0), r_primitive(conn,0,1), c_primitive(end,0))");
    EXPECT_EQ(numerical.evaluate(state1), 3);
    EXPECT_EQ(numerical.evaluate(state2), 3);
    // Shortest distance in union of graphs reduces to 2
    EXPECT_EQ(numerical.evaluate(state), 2);
}
