#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

using namespace dlplan::core;

namespace dlplan::tests::core
{

    TEST(DLPTests, RoleTransitiveReflexiveClosure)
    {
        auto vocabulary = std::make_shared<VocabularyInfo>();
        auto predicate_0 = vocabulary->add_predicate("conn", 2);
        auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
        auto atom_0 = instance->add_atom("conn", {"A", "B"});
        auto atom_1 = instance->add_atom("conn", {"B", "C"});
        auto atom_2 = instance->add_atom("conn", {"C", "A"});
        auto atom_3 = instance->add_atom("conn", {"D", "E"});
        auto atom_4 = instance->add_atom("conn", {"E", "A"});

        State state_0(0, instance, {atom_0, atom_1, atom_2, atom_3, atom_4});

        SyntacticElementFactory factory(vocabulary);

        auto role_0 = factory.parse_role("r_primitive(conn,0,1)");
        EXPECT_EQ(role_0->evaluate(state_0), create_role_denotation(*instance, {{"A", "B"}, {"B", "C"}, {"C", "A"}, {"D", "E"}, {"E", "A"}}));

        auto role_1 = factory.parse_role("r_transitive_reflexive_closure(r_primitive(conn,0,1))");
        // Note that (A, A), (B, B), (C, C) (D, D), (E, E) are included
        EXPECT_EQ(role_1->evaluate(state_0), create_role_denotation(*instance, {{"A", "A"}, {"A", "B"}, {"A", "C"}, {"B", "A"}, {"B", "B"}, {"B", "C"}, {"C", "A"}, {"C", "B"}, {"C", "C"}, {"D", "A"}, {"D", "B"}, {"D", "C"}, {"D", "D"}, {"D", "E"}, {"E", "A"}, {"E", "B"}, {"E", "C"}, {"E", "E"}}));
    }
}
