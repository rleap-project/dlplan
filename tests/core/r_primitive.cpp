#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, RolePrimitive) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("predicate", 3);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0 = instance->add_atom("predicate", {"A", "B", "C"});
    auto atom_1 = instance->add_atom("predicate", {"D", "E", "F"});

    State state_0(0, instance, {atom_0, atom_1});

    SyntacticElementFactory factory(vocabulary);

    auto role_0 = factory.parse_role("r_primitive(predicate,0,1)");
    EXPECT_EQ(role_0->evaluate(state_0), create_role_denotation(*instance, {{"A", "B"}, {"D", "E"}}));

    auto role_1 = factory.parse_role("r_primitive(predicate,1,2)");
    EXPECT_EQ(role_1->evaluate(state_0), create_role_denotation(*instance, {{"B", "C"}, {"E", "F"}}));

    auto role_2 = factory.parse_role("r_primitive(predicate,2,0)");
    EXPECT_EQ(role_2->evaluate(state_0), create_role_denotation(*instance, {{"C", "A"}, {"F", "D"}}));
}

}
