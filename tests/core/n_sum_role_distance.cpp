#include <limits>

#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, NumericalSumRoleDistance) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("conn", 2);
    Predicate p1 = vocabulary->add_predicate("start", 2);
    Predicate p2 = vocabulary->add_predicate("end", 2);
    Predicate p3 = vocabulary->add_predicate("start2", 2);
    Predicate p4 = vocabulary->add_predicate("end2", 2);
    Predicate p5 = vocabulary->add_predicate("start3", 2);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    // Add state atoms
    Atom a0 = instance->add_atom("conn", {"A", "B"});
    Atom a1 = instance->add_atom("conn", {"B", "C"});
    Atom a2 = instance->add_atom("conn", {"C", "A"});
    Atom a3 = instance->add_atom("conn", {"D", "E"});
    Atom a4 = instance->add_atom("conn", {"E", "A"});

    Atom a5 = instance->add_atom("start", {"X", "D"});  // distance 2: D -> E -> A
    Atom a6 = instance->add_atom("start", {"X", "C"});  // distance 1: C -> A
    Atom a7 = instance->add_atom("end", {"X", "A"});
    Atom a8 = instance->add_atom("end", {"X", "B"});
    Atom a9 = instance->add_atom("end", {"Y", "A"});
    Atom a10 = instance->add_atom("end", {"Y", "B"});

    Atom a11 = instance->add_atom("start2", {"X", "B"});  // distance INF
    Atom a12 = instance->add_atom("start2", {"X", "D"});  // distance 1: D -> E
    Atom a13 = instance->add_atom("end2", {"X", "E"});

    State state(instance, {a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13}, 0);

    SyntacticElementFactory factory(vocabulary);
    DenotationsCaches caches;

    // All distances are finite
    Numerical numerical = factory.parse_numerical("n_sum_role_distance(r_primitive(start,0,1),r_primitive(conn,0,1),r_primitive(end,0,1))");
    EXPECT_EQ(numerical.evaluate(state), 3);
    EXPECT_EQ(numerical.evaluate(state, caches), 3);
    EXPECT_EQ(numerical.evaluate({state}, caches), 3);

    // If for at least one source there is no reachable target then the sum is defined as infinity
    Numerical numerical2 = factory.parse_numerical("n_sum_role_distance(r_primitive(start2,0,1),r_primitive(conn,0,1),r_primitive(end2,0,1))");
    EXPECT_EQ(numerical2.evaluate(state), std::numeric_limits<int>::max());
    EXPECT_EQ(numerical2.evaluate(state, caches), std::numeric_limits<int>::max());
    EXPECT_EQ(numerical2.evaluate({state}, caches), std::numeric_limits<int>::max());

    // SumRoleDistance with no sources is defined as 0
    Numerical numerical3 = factory.parse_numerical("n_sum_role_distance(r_primitive(start3,0,1),r_primitive(conn,0,1),r_primitive(end2,0,1))");
    EXPECT_EQ(numerical3.evaluate(state), std::numeric_limits<int>::max());
    EXPECT_EQ(numerical3.evaluate(state, caches), std::numeric_limits<int>::max());
    EXPECT_EQ(numerical3.evaluate({state}, caches), std::numeric_limits<int>::max());
}
