#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, ConceptSome) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("role", 2);
    auto predicate_1 = vocabulary->add_predicate("concept", 1);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0 = instance->add_atom("role", {"A", "B"});
    auto atom_1 = instance->add_atom("role", {"A", "C"});
    auto atom_2 = instance->add_atom("role", {"B", "C"});
    auto atom_3 = instance->add_atom("role", {"B", "D"});
    auto atom_4 = instance->add_atom("role", {"E", "D"});
    auto atom_5 = instance->add_atom("concept", {"B"});
    auto atom_6 = instance->add_atom("concept", {"C"});

    State state_0(0, instance, {atom_0, atom_1, atom_2, atom_3, atom_4, atom_5, atom_6});

    SyntacticElementFactory factory(vocabulary);

    auto concept_ = factory.parse_concept("c_some(r_primitive(role,0,1),c_primitive(concept,0))");
    EXPECT_EQ(concept_->evaluate(state_0), create_concept_denotation(*instance, {"A", "B"}));
}

}
