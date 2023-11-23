#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, ConceptNot) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("concept_1", 1);
    auto predicate_1 = vocabulary->add_predicate("concept_2", 1);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0 = instance->add_atom("concept_1", {"A"});
    auto atom_1 = instance->add_atom("concept_1", {"B"});
    auto atom_2 = instance->add_atom("concept_2", {"B"});
    auto atom_3 = instance->add_atom("concept_2", {"C"});

    State state_0(0, instance, {atom_0, atom_1, atom_2, atom_3});

    SyntacticElementFactory factory(vocabulary);

    auto concept_ = factory.parse_concept("c_not(c_primitive(concept_1,0))");
    EXPECT_EQ(concept_->evaluate(state_0), create_concept_denotation(*instance, {"C"}));
}

}
