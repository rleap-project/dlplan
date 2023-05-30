#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, ConceptOneOf) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("concept", 1);
    auto c0 = vocabulary->add_constant("A");
    auto instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    auto atom_0 = instance->add_atom("concept", {"A"});
    auto atom_1 = instance->add_atom("concept", {"B"});

    State state_0(instance, {atom_0, atom_1}, 0);

    SyntacticElementFactory factory(vocabulary);

    auto concept = factory.parse_concept("c_one_of(A)");
    EXPECT_EQ(concept->evaluate(state_0), create_concept_denotation(*instance, {"A"}));
}

}
