#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, RoleCompose) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("conn_1", 2);
    auto predicate_1 = vocabulary->add_predicate("conn_2", 2);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0 = instance->add_atom("conn_1", {"A", "X"});
    auto atom_1 = instance->add_atom("conn_1", {"B", "X"});
    auto atom_2 = instance->add_atom("conn_1", {"B", "Y"});
    auto atom_3 = instance->add_atom("conn_2", {"X", "B"});
    auto atom_4 = instance->add_atom("conn_2", {"X", "C"});
    auto atom_5 = instance->add_atom("conn_2", {"Z", "A"});

    State state_0(0, instance, {atom_0, atom_1, atom_2, atom_3, atom_4, atom_5});

    SyntacticElementFactory factory(vocabulary);

    auto role1 = factory.parse_role("r_compose(r_primitive(conn_1,0,1),r_primitive(conn_2,0,1))");
    EXPECT_EQ(role1->evaluate(state_0), create_role_denotation(*instance, {{"A", "B"}, {"A", "C"}, {"B", "B"}, {"B", "C"}}));
}

}
