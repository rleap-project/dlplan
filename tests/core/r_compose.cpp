#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, RoleCompose) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("conn_1", 2);
    Predicate p1 = vocabulary->add_predicate("conn_2", 2);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    // Add state atoms
    Atom a0 = instance->add_atom("conn_1", {"A", "X"});
    Atom a1 = instance->add_atom("conn_1", {"B", "X"});
    Atom a2 = instance->add_atom("conn_1", {"B", "Y"});

    Atom a3 = instance->add_atom("conn_2", {"X", "B"});
    Atom a4 = instance->add_atom("conn_2", {"X", "C"});
    Atom a5 = instance->add_atom("conn_2", {"Z", "A"});

    State state(instance, {a0, a1, a2, a3, a4, a5}, 0);

    SyntacticElementFactory factory(vocabulary);
    DenotationsCaches caches;

    Role role1 = factory.parse_role("r_compose(r_primitive(conn_1,0,1),r_primitive(conn_2,0,1))");
    EXPECT_EQ(role1.evaluate(state).to_sorted_vector(), IndexPair_Vec({{0, 2}, {0, 4}, {2, 2}, {2, 4}}));
    EXPECT_EQ(role1.evaluate(state, caches)->to_sorted_vector(), IndexPair_Vec({{0, 2}, {0, 4}, {2, 2}, {2, 4}}));
    EXPECT_EQ(role1.evaluate({state}, caches)->to_sorted_vector(), IndexPair_Vec({{0, 2}, {0, 4}, {2, 2}, {2, 4}}));
}
