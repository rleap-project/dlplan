#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, RoleAnd) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("role_1", 2);
    Predicate p1 = vocabulary->add_predicate("role_2", 2);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    // Add state atoms
    Atom a0 = instance->add_atom("role_1", {"A", "X"});
    Atom a1 = instance->add_atom("role_1", {"B", "Y"});

    Atom a2 = instance->add_atom("role_2", {"B", "Y"});
    Atom a3 = instance->add_atom("role_2", {"C", "Z"});

    State state(instance, {a0, a1, a2, a3}, 0);

    SyntacticElementFactory factory(vocabulary);
    DenotationsCaches caches;

    Role role = factory.parse_role("r_and(r_primitive(role_1,0,1),r_primitive(role_2,0,1))");
    EXPECT_EQ(role.evaluate(state).to_sorted_vector(), IndexPair_Vec({{2,3}}));
    EXPECT_EQ(role.evaluate(state, caches)->to_sorted_vector(), IndexPair_Vec({{2,3}}));
    EXPECT_EQ(role.evaluate({state}, caches)->to_sorted_vector(), IndexPair_Vec({{2,3}}));
}
