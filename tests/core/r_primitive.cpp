#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, RolePrimitive) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("predicate", 3);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    // Add state atoms
    Atom a0 = instance->add_atom("predicate", {"A", "B", "C"});
    Atom a1 = instance->add_atom("predicate", {"D", "E", "F"});

    State state(instance, {a0, a1}, 0);

    SyntacticElementFactory factory(vocabulary);
    DenotationsCaches caches;

    Role role1 = factory.parse_role("r_primitive(predicate,0,1)");
    EXPECT_EQ(role1.evaluate(state).to_sorted_vector(), IndexPair_Vec({{0, 1}, {3, 4}}));
    EXPECT_EQ(role1.evaluate(state, caches)->to_sorted_vector(), IndexPair_Vec({{0, 1}, {3, 4}}));
    EXPECT_EQ(role1.evaluate({state}, caches)->to_sorted_vector(), IndexPair_Vec({{0, 1}, {3, 4}}));

    Role role2 = factory.parse_role("r_primitive(predicate,1,2)");
    EXPECT_EQ(role2.evaluate(state).to_sorted_vector(), IndexPair_Vec({{1, 2}, {4, 5}}));
    EXPECT_EQ(role2.evaluate(state, caches)->to_sorted_vector(), IndexPair_Vec({{1, 2}, {4, 5}}));
    EXPECT_EQ(role2.evaluate({state}, caches)->to_sorted_vector(), IndexPair_Vec({{1, 2}, {4, 5}}));

    Role role3 = factory.parse_role("r_primitive(predicate,2,0)");
    EXPECT_EQ(role3.evaluate(state).to_sorted_vector(), IndexPair_Vec({{2, 0}, {5, 3}}));
    EXPECT_EQ(role3.evaluate(state, caches)->to_sorted_vector(), IndexPair_Vec({{2, 0}, {5, 3}}));
    EXPECT_EQ(role3.evaluate({state}, caches)->to_sorted_vector(), IndexPair_Vec({{2, 0}, {5, 3}}));
}
