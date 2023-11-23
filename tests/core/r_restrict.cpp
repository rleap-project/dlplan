#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, RoleRestrict) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("role", 2);
    auto predicate_1 = vocabulary->add_predicate("concept", 1);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0 = instance->add_atom("role", {"A", "B"});
    auto atom_1 = instance->add_atom("role", {"C", "D"});
    auto atom_2 = instance->add_atom("concept", {"B"});

    State state_0(0, instance, {atom_0, atom_1, atom_2});

    SyntacticElementFactory factory(vocabulary);

    auto role = factory.parse_role("r_restrict(r_primitive(role,0,1),c_primitive(concept,0))");
    EXPECT_EQ(role->evaluate(state_0), create_role_denotation(*instance, {{"A", "B"}}));
}

}
