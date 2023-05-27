#include <gtest/gtest.h>

#include "../utils/domain.h"

#include "../../include/dlplan/policy.h"

using namespace std;
using namespace dlplan::core;
using namespace dlplan::policy;


namespace dlplan::tests::policy {

TEST(DLPTests, PolicyBuilderTest) {
    auto vocabulary_info = gripper::construct_vocabulary_info();
    auto syntactic_element_factory = construct_syntactic_element_factory(vocabulary_info);
    PolicyBuilder builder;
    // add some features
    std::shared_ptr<const Boolean> boolean_1 = syntactic_element_factory.parse_boolean("b_empty(r_primitive(at,0,1))");
    std::shared_ptr<const Boolean> boolean_2 = syntactic_element_factory.parse_boolean("b_empty(c_primitive(package, 0))");
    // add some conditions
    std::shared_ptr<const BaseCondition> c_b_pos_1 = builder.add_pos_condition(boolean_1);
    std::shared_ptr<const BaseCondition> c_b_pos_2 = builder.add_pos_condition(boolean_2);
    std::shared_ptr<const BaseCondition> c_b_neg_1 = builder.add_neg_condition(boolean_1);
    std::shared_ptr<const BaseCondition> c_b_neg_2 = builder.add_neg_condition(boolean_2);
    // add some effects
    std::shared_ptr<const BaseEffect> e_b_pos_1 = builder.add_pos_effect(boolean_1);
    std::shared_ptr<const BaseEffect> e_b_pos_2 = builder.add_pos_effect(boolean_2);
    std::shared_ptr<const BaseEffect> e_b_neg_1 = builder.add_neg_effect(boolean_1);
    std::shared_ptr<const BaseEffect> e_b_neg_2 = builder.add_neg_effect(boolean_2);
    std::shared_ptr<const BaseEffect> e_b_bot_1 = builder.add_bot_effect(boolean_1);
    std::shared_ptr<const BaseEffect> e_b_bot_2 = builder.add_bot_effect(boolean_2);
    // Test something here
    // E.g. canonicity
    builder.add_rule({c_b_pos_2}, {e_b_neg_1});
    auto policy = builder.add_policy({builder.add_rule({c_b_pos_2}, {e_b_neg_1})});
    EXPECT_EQ(policy->compute_repr(),
        "(:policy\n"
        "(:rule (:conditions (:c_b_pos \"b_empty(c_primitive(package,0))\")) (:effects (:e_b_neg \"b_empty(r_primitive(at,0,1))\")))\n"
        ")"
    );
}

}
