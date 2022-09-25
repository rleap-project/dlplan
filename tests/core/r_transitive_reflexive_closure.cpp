#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, RoleTransitiveReflexiveClosure) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("conn", 2);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    // Add state atoms
    Atom a0 = instance->add_atom("conn", {"A", "B"});
    Atom a1 = instance->add_atom("conn", {"B", "C"});
    Atom a2 = instance->add_atom("conn", {"C", "A"});
    Atom a3 = instance->add_atom("conn", {"D", "E"});
    Atom a4 = instance->add_atom("conn", {"E", "A"});

    State state(instance, {a0, a1, a2, a3, a4}, 0);

    SyntacticElementFactory factory(vocabulary);
    DenotationsCaches caches;

    Role role1 = factory.parse_role("r_primitive(conn,0,1)");
    EXPECT_EQ(role1.evaluate(state).to_sorted_vector(), IndexPair_Vec({{0, 1}, {1, 2}, {2, 0}, {3, 4}, {4, 0}}));
    EXPECT_EQ(role1.evaluate(state, caches)->to_sorted_vector(), IndexPair_Vec({{0, 1}, {1, 2}, {2, 0}, {3, 4}, {4, 0}}));
    EXPECT_EQ(role1.evaluate({state}, caches)->to_sorted_vector(), IndexPair_Vec({{0, 1}, {1, 2}, {2, 0}, {3, 4}, {4, 0}}));

    Role role2 = factory.parse_role("r_transitive_reflexive_closure(r_primitive(conn,0,1))");
    // Note that (A, A), (B, B), (C, C) (D, D), (E, E) are included
    EXPECT_EQ(role2.evaluate(state).to_sorted_vector(), IndexPair_Vec({{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {4, 0}, {4, 1}, {4, 2}, {4, 4}}));
    EXPECT_EQ(role2.evaluate(state, caches)->to_sorted_vector(), IndexPair_Vec({{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {4, 0}, {4, 1}, {4, 2}, {4, 4}}));
    EXPECT_EQ(role2.evaluate({state}, caches)->to_sorted_vector(), IndexPair_Vec({{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {4, 0}, {4, 1}, {4, 2}, {4, 4}}));
}
