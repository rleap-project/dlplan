#include <limits>

#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;



// Test the transitive closure
TEST(DLPTests, ConceptDistance) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("conn", 2);
    Predicate p1 = vocabulary->add_predicate("start", 1);
    Predicate p2 = vocabulary->add_predicate("end", 1);
    Predicate p3 = vocabulary->add_predicate("start2", 1);
    Predicate p4 = vocabulary->add_predicate("end2", 1);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary);
    // Add state atoms
    Atom a0 = instance->add_atom("conn", {"A", "B"});
    Atom a1 = instance->add_atom("conn", {"B", "C"});
    Atom a2 = instance->add_atom("conn", {"C", "A"});
    Atom a3 = instance->add_atom("conn", {"D", "E"});
    Atom a4 = instance->add_atom("conn", {"E", "A"});

    Atom a5 = instance->add_atom("start", {"D"});
    Atom a6 = instance->add_atom("end", {"A"});
    Atom a7 = instance->add_atom("end", {"B"});

    Atom a8 = instance->add_atom("start2", {"B"});
    Atom a9 = instance->add_atom("end2", {"D"});

    State state(instance, {a0, a1, a2, a3, a4, a5, a6, a7, a8, a9});

    SyntacticElementFactory factory(vocabulary);

    Numerical numerical = factory.parse_numerical("n_concept_distance(start(0), conn(0,1), end(0))");
    EXPECT_EQ(numerical.evaluate(state), 2);

    Numerical numerical2 = factory.parse_numerical("n_concept_distance(start2(0), conn(0,1), end2(0))");
    EXPECT_EQ(numerical2.evaluate(state), std::numeric_limits<int>::max());
}
