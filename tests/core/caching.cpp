#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

using namespace dlplan::core;

namespace dlplan::tests::core
{
    TEST(DLPTests, Caching)
    {
        auto vocabulary = std::make_shared<VocabularyInfo>();
        auto predicate_0 = vocabulary->add_predicate("role", 2);
        auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
        auto atom_0 = instance->add_atom("role", {"A", "B"});

        State state_0(0, instance, std::vector<Atom>{});
        State state_1(1, instance, {atom_0});

        SyntacticElementFactory factory(vocabulary);
        DenotationsCaches caches;

        auto concept_0 = factory.parse_concept("c_primitive(role, 0)");
        EXPECT_EQ(concept_0->evaluate(state_0), create_concept_denotation(*instance, {}));
        EXPECT_EQ(concept_0->evaluate(state_0), *concept_0->evaluate(state_0, caches));
        EXPECT_EQ(concept_0->evaluate(state_0, caches), concept_0->evaluate(state_0, caches));
        EXPECT_EQ(concept_0->evaluate(state_1), create_concept_denotation(*instance, {"A"}));
        EXPECT_EQ(concept_0->evaluate(state_1), *concept_0->evaluate(state_1, caches));
        EXPECT_EQ(concept_0->evaluate(state_1, caches), concept_0->evaluate(state_1, caches));
        EXPECT_EQ(
            ConceptDenotations({concept_0->evaluate(state_0,caches), concept_0->evaluate(state_1,caches)}),
            *concept_0->evaluate(States{state_0, state_1}, caches)
        );

        auto role_0 = factory.parse_role("r_primitive(role, 0, 1)");
        EXPECT_EQ(role_0->evaluate(state_0), create_role_denotation(*instance, {}));
        EXPECT_EQ(role_0->evaluate(state_0), *role_0->evaluate(state_0, caches));
        EXPECT_EQ(role_0->evaluate(state_0, caches), role_0->evaluate(state_0, caches));
        EXPECT_EQ(role_0->evaluate(state_1), create_role_denotation(*instance, {{"A", "B"}}));
        EXPECT_EQ(role_0->evaluate(state_1), *role_0->evaluate(state_1, caches));
        EXPECT_EQ(role_0->evaluate(state_1, caches), role_0->evaluate(state_1, caches));
        EXPECT_EQ(
            RoleDenotations({role_0->evaluate(state_0,caches), role_0->evaluate(state_1,caches)}),
            *role_0->evaluate(States{state_0, state_1}, caches)
        );

        auto numerical_0 = factory.parse_numerical("n_count(c_primitive(role, 0))");
        EXPECT_EQ(numerical_0->evaluate(state_0), 0);
        EXPECT_EQ(numerical_0->evaluate(state_0), numerical_0->evaluate(state_0, caches));
        EXPECT_EQ(numerical_0->evaluate(state_1), 1);
        EXPECT_EQ(numerical_0->evaluate(state_1), numerical_0->evaluate(state_1, caches));
        EXPECT_EQ(
            NumericalDenotations({numerical_0->evaluate(state_0), numerical_0->evaluate(state_1)}),
            *numerical_0->evaluate(States{state_0, state_1}, caches)
        );
        EXPECT_EQ(numerical_0->evaluate(States{state_0, state_1}, caches), numerical_0->evaluate(States{state_0, state_1}, caches));

        auto boolean_0 = factory.parse_boolean("b_empty(c_primitive(role, 0))");
        EXPECT_EQ(boolean_0->evaluate(state_0), true);
        EXPECT_EQ(boolean_0->evaluate(state_0), boolean_0->evaluate(state_0, caches));
        EXPECT_EQ(boolean_0->evaluate(state_1), false);
        EXPECT_EQ(boolean_0->evaluate(state_1), boolean_0->evaluate(state_1, caches));
        EXPECT_EQ(
            BooleanDenotations({boolean_0->evaluate(state_0), boolean_0->evaluate(state_1)}),
            *boolean_0->evaluate(States{state_0, state_1}, caches)
        );
        EXPECT_EQ(boolean_0->evaluate(States{state_0, state_1}, caches), boolean_0->evaluate(States{state_0, state_1}, caches));
    }
}
