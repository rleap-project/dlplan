#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, RoleNot) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("role", 2);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    // Add state atoms
    Atom a0 = instance->add_atom("role", {"A", "B"});

    State state(instance, {a0}, 0);

    SyntacticElementFactory factory(vocabulary);
    DenotationsCaches caches;

    Role role = factory.parse_role("r_not(r_primitive(role,0,1))");
    EXPECT_EQ(role.evaluate(state).to_sorted_vector(), IndexPair_Vec({{0,0},{1,0},{1,1}}));
    EXPECT_EQ(role.evaluate(state, caches)->to_sorted_vector(), IndexPair_Vec({{0,0},{1,0},{1,1}}));
    EXPECT_EQ(role.evaluate({state}, caches)->to_sorted_vector(), IndexPair_Vec({{0,0},{1,0},{1,1}}));
}
