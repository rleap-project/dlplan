#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, RolePrimitive) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("predicate", 3);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary);
    // Add state atoms
    Atom a0 = instance->add_atom("predicate", {"A", "B", "C"});
    Atom a1 = instance->add_atom("predicate", {"D", "E", "F"});


    State state(instance, {a0, a1});

    SyntacticElementFactory factory(vocabulary);

    Role role1 = factory.parse_role("predicate(0,1)");
    EXPECT_EQ(role1.evaluate(state), dlplan::core::RoleDenotation({{0, 1}, {3, 4}}));

    Role role2 = factory.parse_role("predicate(1,2)");
    EXPECT_EQ(role2.evaluate(state), dlplan::core::RoleDenotation({{1, 2}, {4, 5}}));

    Role role3 = factory.parse_role("predicate(2,0)");
    EXPECT_EQ(role3.evaluate(state), dlplan::core::RoleDenotation({{2, 0}, {5, 3}}));
}
