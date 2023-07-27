#include <gtest/gtest.h>

#include "../../include/dlplan/state_space.h"
#include "../../include/dlplan/serialization.h"

#include <sstream>


namespace dlplan::serialization::tests {

TEST(DLPTests, SerializationTestEmpty) {
    std::stringstream buffer;
    dlplan::serialization::Data out_data{};
    dlplan::serialization::serialize(out_data, buffer);
    dlplan::serialization::Data in_data = dlplan::serialization::deserialize(buffer);
    EXPECT_EQ(in_data.m_state_spaces.size(), 0);
}


TEST(DLPTests, SerializationTest1) {
    std::stringstream buffer;
    dlplan::serialization::Data out_data{};
    auto vocabulary_info = std::make_shared<dlplan::core::VocabularyInfo>();
    vocabulary_info->add_constant("c1");
    auto predicate_0 = vocabulary_info->add_predicate("nullary_predicate", 0);
    auto predicate_1 = vocabulary_info->add_predicate("unary_predicate", 1);
    auto predicate_2 = vocabulary_info->add_predicate("binary_predicate", 2);
    auto instance_info_1 = std::make_shared<dlplan::core::InstanceInfo>(vocabulary_info, 0);
    auto atom_1_0 = instance_info_1->add_atom("nullary_predicate", {});
    auto atom_1_1 = instance_info_1->add_atom("unary_predicate", {"A"});
    auto instance_info_2 = std::make_shared<dlplan::core::InstanceInfo>(vocabulary_info, 1);
    auto atom_2_0 = instance_info_2->add_atom("nullary_predicate", {});
    auto atom_2_1 = instance_info_2->add_atom("unary_predicate", {"A"});
    auto atom_2_2 = instance_info_2->add_atom("unary_predicate", {"B"});
    // TODO: cover more branches.
    auto state_space_1 = std::make_unique<dlplan::state_space::StateSpace>(
        instance_info_1,
        dlplan::state_space::StateMapping{{0, dlplan::core::State(instance_info_1, {atom_1_0}, 0)}},
        0,
        dlplan::state_space::AdjacencyList{},
        dlplan::state_space::StateIndicesSet{});
    auto state_space_2 = std::make_unique<dlplan::state_space::StateSpace>(
        instance_info_2,
        dlplan::state_space::StateMapping{{0, dlplan::core::State(instance_info_2, {atom_2_0}, 0)}},
        0,
        dlplan::state_space::AdjacencyList{},
        dlplan::state_space::StateIndicesSet{});
    out_data.m_state_spaces.push_back(std::move(state_space_1));
    out_data.m_state_spaces.push_back(std::move(state_space_2));
    dlplan::serialization::serialize(out_data, buffer);
    dlplan::serialization::Data in_data = dlplan::serialization::deserialize(buffer);
    // TODO: add more assertions
    EXPECT_EQ(in_data.m_state_spaces.size(), 2);
}

}
