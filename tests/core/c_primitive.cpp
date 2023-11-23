#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, ConceptPrimitive) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("predicate", 3);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0 = instance->add_atom("predicate", {"A", "B", "C"});
    auto atom_1 = instance->add_atom("predicate", {"D", "E", "F"});

    State state_0(0, instance, {atom_0, atom_1});

    SyntacticElementFactory factory(vocabulary);

    auto concept1 = factory.parse_concept("c_primitive(predicate,0)");
    EXPECT_EQ(concept1->evaluate(state_0), create_concept_denotation(*instance, {"A", "D"}));

    auto concept2 = factory.parse_concept("c_primitive(predicate,1)");
    EXPECT_EQ(concept2->evaluate(state_0), create_concept_denotation(*instance, {"B", "E"}));

    auto concept3 = factory.parse_concept("c_primitive(predicate,2)");
    EXPECT_EQ(concept3->evaluate(state_0), create_concept_denotation(*instance, {"C", "F"}));
}

}
