#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, RoleAnd) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("role_1", 2);
    auto predicate_1 = vocabulary->add_predicate("role_2", 2);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0 = instance->add_atom("role_1", {"A", "X"});
    auto atom_1 = instance->add_atom("role_1", {"B", "Y"});
    auto atom_2 = instance->add_atom("role_2", {"B", "Y"});
    auto atom_3 = instance->add_atom("role_2", {"C", "Z"});

    State state_0(0, instance, {atom_0, atom_1, atom_2, atom_3});

    SyntacticElementFactory factory(vocabulary);

    auto role = factory.parse_role("r_and(r_primitive(role_1,0,1),r_primitive(role_2,0,1))");
    EXPECT_EQ(role->evaluate(state_0), create_role_denotation(*instance, {{"B", "Y"}}));
}

}
