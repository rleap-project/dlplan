#include <gtest/gtest.h>

#include "../utils/domain.h"

#include "../../include/dlplan/policy.h"

using namespace std;
using namespace dlplan::policy;


namespace dlplan::tests::policy {

TEST(DLPTests, PolicyBuilderTest) {
    auto vocabulary_info = gripper::construct_vocabulary_info();
    auto element_factory = construct_syntactic_element_factory(vocabulary_info);
    PolicyFactory policy_factory(element_factory);
    // add some features
    std::shared_ptr<const NamedBoolean> boolean_1 = policy_factory.make_boolean("b0", element_factory->parse_boolean("b_empty(r_primitive(at,0,1))"));
    std::shared_ptr<const NamedBoolean> boolean_2 = policy_factory.make_boolean("b1", element_factory->parse_boolean("b_empty(c_primitive(package, 0))"));
    // add some conditions
    std::shared_ptr<const BaseCondition> c_b_pos_1 = policy_factory.make_pos_condition(boolean_1);
    std::shared_ptr<const BaseCondition> c_b_pos_2 = policy_factory.make_pos_condition(boolean_2);
    std::shared_ptr<const BaseCondition> c_b_neg_1 = policy_factory.make_neg_condition(boolean_1);
    std::shared_ptr<const BaseCondition> c_b_neg_2 = policy_factory.make_neg_condition(boolean_2);
    // add some effects
    std::shared_ptr<const BaseEffect> e_b_pos_1 = policy_factory.make_pos_effect(boolean_1);
    std::shared_ptr<const BaseEffect> e_b_pos_2 = policy_factory.make_pos_effect(boolean_2);
    std::shared_ptr<const BaseEffect> e_b_neg_1 = policy_factory.make_neg_effect(boolean_1);
    std::shared_ptr<const BaseEffect> e_b_neg_2 = policy_factory.make_neg_effect(boolean_2);
    std::shared_ptr<const BaseEffect> e_b_bot_1 = policy_factory.make_bot_effect(boolean_1);
    std::shared_ptr<const BaseEffect> e_b_bot_2 = policy_factory.make_bot_effect(boolean_2);
    // Test something here
    // E.g. canonicity
    policy_factory.make_rule({c_b_pos_2}, {e_b_neg_1});
    auto policy = policy_factory.make_policy({policy_factory.make_rule({c_b_pos_2}, {e_b_neg_1})});
    EXPECT_EQ(policy->str(),
        "(:policy\n"
        "(:booleans (b1 \"b_empty(c_primitive(package,0))\") (b0 \"b_empty(r_primitive(at,0,1))\"))\n"
        "(:numericals )\n"
        "(:rule (:conditions (:c_b_pos b1)) (:effects (:e_b_neg b0)))\n"
        ")"
    );
}

}
