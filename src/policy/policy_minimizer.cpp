#include "condition.h"
#include "effect.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {

PolicyMinimizer::PolicyMinimizer() { }

PolicyMinimizer::PolicyMinimizer(const PolicyMinimizer& other) = default;

PolicyMinimizer& PolicyMinimizer::operator=(const PolicyMinimizer& other) = default;

PolicyMinimizer::PolicyMinimizer(PolicyMinimizer&& other) = default;

PolicyMinimizer& PolicyMinimizer::operator=(PolicyMinimizer&& other) = default;

PolicyMinimizer::~PolicyMinimizer() { }

std::unordered_set<std::shared_ptr<const Rule>> PolicyMinimizer::try_merge_by_condition(const Policy& policy, PolicyBuilder& builder) const {
    for (const auto& rule_1 : policy.get_rules()) {
        for (const auto& rule_2 : policy.get_rules()) {
            if (rule_1->get_index() >= rule_2->get_index()) {
                continue;
            }
            if (rule_1->get_effects() != rule_2->get_effects()) {
                continue;
            }
            std::unordered_set<std::shared_ptr<const BaseCondition>> diff = compute_mergeable_values<BaseCondition, BooleanCondition, NumericalCondition>({rule_1->get_conditions(), rule_2->get_conditions()});
            if (diff.empty()) {
                continue;
            }
            builder.add_rule(compute_merged_values(rule_1->get_conditions(), diff, builder), compute_merged_values(rule_1->get_effects(), {}, builder));
            return {rule_1, rule_2};
        }
    }
    return {};
}

std::unordered_set<std::shared_ptr<const Rule>> PolicyMinimizer::try_merge_by_effect(const Policy& policy, PolicyBuilder& builder) const {
    for (const auto& rule_1 : policy.get_rules()) {
        for (const auto& rule_2 : policy.get_rules()) {
            if (rule_1->get_index() >= rule_2->get_index()) {
                continue;
            }
            if (rule_1->get_conditions() != rule_2->get_conditions()) {
                continue;
            }
            std::unordered_set<std::shared_ptr<const BaseEffect>> diff = compute_mergeable_values<BaseEffect, BooleanEffect, NumericalEffect>({rule_1->get_effects(), rule_2->get_effects()});
            if (diff.empty()) {
                continue;
            }
            for (const auto& rule_3 : policy.get_rules()) {
                if (rule_2->get_index() >= rule_3->get_index()) {
                    continue;
                }
                if (rule_2->get_conditions() != rule_3->get_conditions()) {
                    continue;
                }
                diff = compute_mergeable_values<BaseEffect, BooleanEffect, NumericalEffect>({rule_1->get_effects(), rule_2->get_effects(), rule_3->get_effects()});
                if (diff.empty()) {
                    continue;
                }
                builder.add_rule(compute_merged_values(rule_1->get_conditions(), {}, builder), compute_merged_values(rule_1->get_effects(), diff, builder));
                return {rule_1, rule_2, rule_3};
            }
        }
    }
    return {};
}

Policy PolicyMinimizer::minimize_greedy(const Policy& policy) const {
    Policy current_policy = policy;
    std::unordered_set<std::shared_ptr<const Rule>> merged_rules;
    do {
        PolicyBuilder builder;
        merged_rules = try_merge_by_condition(current_policy, builder);
        if (merged_rules.empty()) {
            merged_rules = try_merge_by_effect(current_policy, builder);
        }
        for (const auto& rule : current_policy.get_rules()) {
            if (merged_rules.count(rule)) {
                continue;
            }
            rule->visit(builder);
        }
        current_policy = builder.get_result();
    } while (!merged_rules.empty());

    return current_policy;
}

Policy PolicyMinimizer::minimize_greedy(const Policy& policy, const core::StatePairs& true_state_pairs, const core::StatePairs& false_state_pairs) const {
    /*
       Idea: A rule C -> E dominates a rules C' -> E' if C subseteq C', E subseteq E'
             and substituting C' -> E' with C -> E in policy leads to consistent classification.
    */
    /* 1. try to remove conditions such that classification remains.
    */
    /* 2. try to remove effects such that classification remains.
    */
    throw std::runtime_error("Not implemented.");
}

}