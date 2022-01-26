#include "../include/dlplan/policy.h"

#include <fstream>
#include <cassert>
#include <sstream>

#include "../include/dlplan/core.h"


using namespace dlplan::policy;

int main() {
    // Construct SyntacticElementFactory
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary_info = std::make_shared<dlplan::core::VocabularyInfo>();
    vocabulary_info->add_predicate("unary", 1);
    dlplan::core::SyntacticElementFactory factory(vocabulary_info);
    dlplan::core::Boolean boolean = factory.parse_boolean("b_empty(c_primitive(unary,0))");
    dlplan::core::Numerical numerical = factory.parse_numerical("n_count(c_primitive(unary,0))");

    // Construct the empty policy.
    PolicyBuilder builder;
    // Add features.
    std::shared_ptr<const BooleanFeature> b = builder.add_boolean_feature(boolean);
    std::shared_ptr<const NumericalFeature> n = builder.add_numerical_feature(numerical);
    // Add conditions and effects the rule.
    std::shared_ptr<const BaseCondition> b_neg_condition_0 = builder.add_b_neg_condition(b);
    std::shared_ptr<const BaseEffect> b_bot_effect_0 = builder.add_b_bot_effect(b);
    std::shared_ptr<const BaseCondition> n_gt_condition_0 = builder.add_n_gt_condition(n);
    std::shared_ptr<const BaseEffect> n_dec_effect_0 = builder.add_n_dec_effect(n);
    // Add rule.
    std::shared_ptr<const Rule> r = builder.add_rule(
        {b_neg_condition_0, n_gt_condition_0},
        {b_bot_effect_0, n_dec_effect_0}
    );
    Policy policy = builder.get_result();

    // Construct InstanceInfo and States
    std::shared_ptr<dlplan::core::InstanceInfo> instance_info = std::make_shared<dlplan::core::InstanceInfo>(vocabulary_info);
    const auto a0 = instance_info->add_atom("unary", {"A"});
    const auto a1 = instance_info->add_atom("unary", {"B"});
    State s0(0, dlplan::core::State(instance_info, std::vector<dlplan::core::Atom>()));
    State s1(1, dlplan::core::State(instance_info, {a0}));
    State s2(2, dlplan::core::State(instance_info, {a0, a1}));

    // Evaluate the policy.
    assert(policy.evaluate(s2, s1).second);
    assert(!policy.evaluate(s2, s0).second);
    assert(!policy.evaluate(s1, s2).second);
    assert(!policy.evaluate(s0, s2).second);

    // Write policy to file.
    std::cout << "Write policy:" << std::endl;
    std::cout << policy.compute_repr() << std::endl << std::endl;
    std::ofstream ofs;
    ofs.open("test.txt", std::ofstream::out);
    ofs << PolicyWriter().write(policy);
    ofs.close();

    // Read policy from file.
    std::ifstream ifs;
    ifs.open("test.txt", std::ifstream::in);
    std::stringstream ss;
    ss << ifs.rdbuf();
    Policy policy_in = PolicyReader().read(ss.str(), factory);
    ifs.close();
    std::cout << "Read policy:" << std::endl;
    std::cout << policy_in.compute_repr() << std::endl << std::endl;

    return 0;
}
