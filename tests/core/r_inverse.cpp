#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, RoleInverse) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("role", 2);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0 = instance->add_atom("role", {"A", "B"});
    auto atom_1 = instance->add_atom("role", {"C", "D"});

    State state_0(0, instance, {atom_0, atom_1});

    SyntacticElementFactory factory(vocabulary);

    auto role_0 = factory.parse_role("r_inverse(r_primitive(role,0,1))");
    EXPECT_EQ(role_0->evaluate(state_0), create_role_denotation(*instance, {{"B","A"},{"D","C"}}));
}

}
