#include <gtest/gtest.h>

#include "include/dlplan/core.h"
#include "include/dlplan/novelty.h"
#include "include/dlplan/policy.h"
#include "include/dlplan/state_space.h"
#include "include/dlplan/serialization.h"

#include <sstream>

using namespace dlplan::core;
using namespace dlplan::novelty;
using namespace dlplan::policy;
using namespace dlplan::state_space;
using namespace dlplan::serialization;

namespace dlplan::serialization::tests {

TEST(DLPTests, SerializationGripperTest) {
    /* Serialization */
    auto result_1 = generate_state_space("domain.pddl", "p-1-0.pddl", nullptr, 0);
    std::shared_ptr<StateSpace> state_space_1 = std::move(result_1.state_space);
    auto result_2 = generate_state_space("domain.pddl", "p-2-0.pddl", state_space_1->get_instance_info()->get_vocabulary_info(), 1);
    std::shared_ptr<StateSpace> state_space_2 = std::move(result_2.state_space);
    Data out_data;

    // StateSpace
    out_data.state_spaces.emplace("0", state_space_1);
    out_data.state_spaces.emplace("1", state_space_2);

    // TupleGraph
    auto novelty_base = std::make_shared<NoveltyBase>(state_space_1->get_instance_info()->get_atoms().size(), 1);
    for (const auto& pair : state_space_1->get_states()) {
        out_data.tuple_graphs.emplace(std::to_string(state_space_1->get_instance_info()->get_index()) + "_" + std::to_string(pair.first), std::make_shared<TupleGraph>(novelty_base, state_space_1, pair.first));
    }
    novelty_base = std::make_shared<NoveltyBase>(state_space_2->get_instance_info()->get_atoms().size(), 1);
    for (const auto& pair : state_space_2->get_states()) {
        out_data.tuple_graphs.emplace(std::to_string(state_space_2->get_instance_info()->get_index()) + "_" + std::to_string(pair.first), std::make_shared<TupleGraph>(novelty_base, state_space_2, pair.first));
    }

    // VocabularyInfo
    out_data.vocabulary_infos.emplace("0", state_space_1->get_instance_info()->get_vocabulary_info());
    out_data.vocabulary_infos.emplace("1", state_space_2->get_instance_info()->get_vocabulary_info());

    // InstanceData
    out_data.instance_infos.emplace("0", state_space_1->get_instance_info());
    out_data.instance_infos.emplace("1", state_space_2->get_instance_info());

    // SyntacticElementFactory
    auto element_factory = std::make_shared<SyntacticElementFactory>(state_space_1->get_instance_info()->get_vocabulary_info());
    auto numerical = element_factory->parse_numerical("n_count(c_primitive(free, 0))");
    auto boolean = element_factory->parse_boolean("b_empty(r_and(r_primitive(at, 0, 1), r_primitive(at_g, 0, 1)))");
    out_data.syntatic_element_factories.emplace("0", element_factory);

    // PolicyFactory
    auto policy_factory = std::make_shared<PolicyFactory>(element_factory);
    auto c_n_gt = policy_factory->make_gt_condition(numerical);
    auto e_n_dec = policy_factory->make_dec_effect(numerical);
    auto c_b_pos = policy_factory->make_pos_condition(boolean);
    auto e_b_neg = policy_factory->make_neg_effect(boolean);
    auto rule_1 = policy_factory->make_rule({c_n_gt}, {e_n_dec});
    auto rule_2 = policy_factory->make_rule({c_b_pos}, {e_b_neg});
    auto policy = policy_factory->make_policy({rule_1, rule_2});
    out_data.policy_factories.emplace("0", policy_factory);

    // Policy
    out_data.policies.emplace("0", policy);

    // DenotationsCaches
    auto denotations_caches_1 = std::make_shared<DenotationsCaches>();
    for (const auto& pair : state_space_1->get_states()) {
        numerical->evaluate(pair.second, *denotations_caches_1);
    }
    auto denotations_caches_2 = std::make_shared<DenotationsCaches>();
    for (const auto& pair : state_space_2->get_states()) {
        numerical->evaluate(pair.second, *denotations_caches_2);
    }
    out_data.denotations_caches.emplace("0", denotations_caches_1);
    out_data.denotations_caches.emplace("1", denotations_caches_2);

    /* Deserialization */
    std::stringstream buffer;
    dlplan::serialization::serialize(out_data, buffer);
    std::cout << buffer.str() << std::endl;

    dlplan::serialization::Data in_data = dlplan::serialization::deserialize(buffer);
    EXPECT_EQ(in_data.state_spaces.size(), 2);
    EXPECT_NE(in_data.state_spaces.at("0")->get_instance_info(), in_data.state_spaces.at("1")->get_instance_info());
    EXPECT_EQ(in_data.state_spaces.at("0")->get_instance_info()->get_vocabulary_info(), in_data.state_spaces.at("1")->get_instance_info()->get_vocabulary_info());

    EXPECT_EQ(in_data.tuple_graphs.size(), state_space_1->get_states().size() + state_space_2->get_states().size());
    EXPECT_EQ(in_data.tuple_graphs.count("0_0"), 1);
    EXPECT_EQ(in_data.tuple_graphs.count("1_0"), 1);

    EXPECT_EQ(in_data.vocabulary_infos.size(), 2);
    EXPECT_EQ(in_data.vocabulary_infos["0"], in_data.vocabulary_infos["1"]);

    EXPECT_EQ(in_data.instance_infos.size(), 2);
    EXPECT_NE(in_data.instance_infos["0"], in_data.instance_infos["1"]);

    EXPECT_EQ(in_data.denotations_caches.size(), 2);

    EXPECT_EQ(in_data.syntatic_element_factories.size(), 1);

    EXPECT_EQ(in_data.policies.size(), 1);

    EXPECT_EQ(in_data.policy_factories.size(), 1);
}

}
