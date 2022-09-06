#include <gtest/gtest.h>

#include "utils.h"

#include "../include/dlplan/policy.h"

using namespace dlplan::core;
using namespace dlplan::policy;


TEST(DLPTests, PolicyBuilderTest) {
    auto vocabulary_info = construct_vocabulary_info();
    auto syntactic_element_factory = construct_syntactic_element_factory(vocabulary_info);
    PolicyBuilder builder;
    // add some features
    std::shared_ptr<const Boolean> boolean_1 = builder.add_boolean_feature(syntactic_element_factory.parse_boolean("b_empty(r_primitive(at,0,1))"));
    std::shared_ptr<const Boolean> boolean_2 = builder.add_boolean_feature(syntactic_element_factory.parse_boolean("b_empty(c_primitive(package, 0))"));
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
}
