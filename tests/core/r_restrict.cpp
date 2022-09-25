#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, RoleRestrict) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("role", 2);
    Predicate p1 = vocabulary->add_predicate("concept", 1);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    // Add state atoms
    Atom a0 = instance->add_atom("role", {"A", "B"});
    Atom a1 = instance->add_atom("role", {"C", "D"});

    Atom a2 = instance->add_atom("concept", {"B"});

    State state(instance, {a0, a1, a2}, 0);

    SyntacticElementFactory factory(vocabulary);
    DenotationsCaches caches;

    Role role = factory.parse_role("r_restrict(r_primitive(role,0,1),c_primitive(concept,0))");
    EXPECT_EQ(role.evaluate(state).to_sorted_vector(), IndexPair_Vec({{0, 1}}));
    EXPECT_EQ(role.evaluate(state, caches)->to_sorted_vector(), IndexPair_Vec({{0, 1}}));
    EXPECT_EQ(role.evaluate({state}, caches)->to_sorted_vector(), IndexPair_Vec({{0, 1}}));
}
