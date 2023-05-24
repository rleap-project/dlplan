#include "../include/dlplan/policy.h"

#include <fstream>
#include <cassert>
#include <sstream>

#include "../include/dlplan/core.h"


using namespace dlplan::policy;
using namespace dlplan::core;

int main() {
    // Construct SyntacticElementFactory
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary_info = std::make_shared<dlplan::core::VocabularyInfo>();
    vocabulary_info->add_predicate("unary", 1);

    dlplan::core::SyntacticElementFactory factory(vocabulary_info);
    std::shared_ptr<const Boolean> boolean_1 = factory.parse_boolean("b_empty(c_primitive(unary,0))");
    std::shared_ptr<const Numerical> numerical_1 = factory.parse_numerical("n_count(c_primitive(unary,0))");

    // Construct the empty policy.
    PolicyBuilder builder;
    // Add conditions and effects the rule.
    std::shared_ptr<const BaseCondition> b_neg_condition_0 = builder.add_neg_condition(boolean_1);
    std::shared_ptr<const BaseEffect> b_bot_effect_0 = builder.add_bot_effect(boolean_1);
    std::shared_ptr<const BaseCondition> n_gt_condition_0 = builder.add_gt_condition(numerical_1);
    std::shared_ptr<const BaseEffect> n_dec_effect_0 = builder.add_dec_effect(numerical_1);
    // Add rule.
    std::shared_ptr<const Rule> rule_1 = builder.add_rule(
        {b_neg_condition_0, n_gt_condition_0},
        {b_bot_effect_0, n_dec_effect_0}
    );
    std::shared_ptr<const Policy> policy = builder.add_policy({rule_1});

    // Construct InstanceInfo and States
    std::shared_ptr<dlplan::core::InstanceInfo> instance_info = std::make_shared<dlplan::core::InstanceInfo>(vocabulary_info);
    const auto atom_0 = instance_info->add_atom("unary", {"A"});
    const auto atom_1 = instance_info->add_atom("unary", {"B"});
    dlplan::core::State state_1(instance_info, std::vector<dlplan::core::Atom>(), 0);
    dlplan::core::State state_2(instance_info, {atom_0}, 1);
    dlplan::core::State state_3(instance_info, {atom_0, atom_1}, 2);

    // Construct a cache to avoid redundant reevaluations
    dlplan::core::DenotationsCaches caches;

    // Evaluate the policy using the cache
    assert(policy->evaluate(state_3, state_2, caches));
    assert(!policy->evaluate(state_3, state_1, caches));
    assert(!policy->evaluate(state_2, state_3, caches));
    assert(!policy->evaluate(state_1, state_3, caches));
    // Evaluate the policy without the cache
    assert(policy->evaluate(state_3, state_2));
    assert(!policy->evaluate(state_3, state_1));
    assert(!policy->evaluate(state_2, state_3));
    assert(!policy->evaluate(state_1, state_3));

    // Write policy to file.
    std::cout << "Write policy:" << std::endl;
    std::cout << policy->compute_repr() << std::endl << std::endl;
    std::cout << policy->str() << std::endl << std::endl;
    std::ofstream ofs;
    ofs.open("test.txt", std::ofstream::out);
    ofs << PolicyWriter().write(*policy);
    ofs.close();

    // Read policy from file.
    std::ifstream ifs;
    ifs.open("test.txt", std::ifstream::in);
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::shared_ptr<const Policy> policy_in = PolicyReader().read(ss.str(), builder, factory);
    ifs.close();
    std::cout << "Read policy:" << std::endl;
    std::cout << policy_in->compute_repr() << std::endl << std::endl;
    std::cout << policy_in->str() << std::endl << std::endl;
    return 0;
}
