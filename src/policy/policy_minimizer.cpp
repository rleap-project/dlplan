#include "condition.h"
#include "effect.h"

#include "../utils/set_operators.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {

PolicyMinimizer::PolicyMinimizer() { }

PolicyMinimizer::PolicyMinimizer(const PolicyMinimizer& other) = default;

PolicyMinimizer& PolicyMinimizer::operator=(const PolicyMinimizer& other) = default;

PolicyMinimizer::PolicyMinimizer(PolicyMinimizer&& other) = default;

PolicyMinimizer& PolicyMinimizer::operator=(PolicyMinimizer&& other) = default;

PolicyMinimizer::~PolicyMinimizer() { }

bool PolicyMinimizer::check_policy_matches_classification(const Policy& policy, const core::StatePairs& true_state_pairs, const core::StatePairs& false_state_pairs) const {
    for (const auto& state_pair : true_state_pairs) {
        if (!policy.evaluate_lazy(state_pair.first, state_pair.second)) {
            return false;
        }
    }
    for (const auto& state_pair : false_state_pairs) {
        if (policy.evaluate_lazy(state_pair.first, state_pair.second)) {
            return false;
        }
    }
    return true;
}

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

Policy PolicyMinimizer::minimize(const Policy& policy) const {
    // Merge rules
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
    // Remove dominated rules
    std::unordered_set<std::shared_ptr<const Rule>> dominated_rules;
    for (const auto& rule_1 : current_policy.get_rules()) {
        auto rule_1_conditions = rule_1->get_conditions();
        auto rule_1_effects = rule_1->get_effects();
        std::unordered_set<std::shared_ptr<const BaseCondition>> rule_1_conditions_set(rule_1_conditions.begin(), rule_1_conditions.end());
        std::unordered_set<std::shared_ptr<const BaseEffect>> rule_1_effects_set(rule_1_effects.begin(), rule_1_effects.end());
        for (const auto& rule_2 : current_policy.get_rules()) {
            if (rule_1 == rule_2) {
                continue;
            }
            auto rule_2_conditions = rule_2->get_conditions();
            auto rule_2_effects = rule_2->get_effects();
            std::unordered_set<std::shared_ptr<const BaseCondition>> rule_2_conditions_set(rule_2_conditions.begin(), rule_2_conditions.end());
            std::unordered_set<std::shared_ptr<const BaseEffect>> rule_2_effects_set(rule_2_effects.begin(), rule_2_effects.end());
            if (utils::is_subset_eq(rule_1_conditions_set, rule_2_conditions_set) && utils::is_subset_eq(rule_1_effects_set, rule_2_effects_set)) {
                dominated_rules.insert(rule_2);
                break;
            }
        }
    }
    PolicyBuilder builder;
    for (const auto& rule : current_policy.get_rules()) {
        if (dominated_rules.count(rule)) {
            continue;
        }
        rule->visit(builder);
    }
    current_policy = builder.get_result();
    return current_policy;
}

Policy PolicyMinimizer::minimize(const Policy& policy, const core::StatePairs& true_state_pairs, const core::StatePairs& false_state_pairs) const {
    /*
       Idea: A rule C -> E dominates a rules C' -> E' if C subseteq C', E subseteq E'
             and substituting C' -> E' with C -> E in policy leads to consistent classification.
    */
    /* 1. try to remove conditions such that classification remains.
    */
    /* 2. try to remove effects such that classification remains.
    */
    /*Policy current_policy = policy;
    // To skip faster to unchecked rules, conditions and effects.
    std::unordered_set<std::string> finished_rule_reprs;
    std::unordered_map<std::string, std::unordered_set<std::string>> rule_repr_to_finished_value_reprs;
    do {
        bool minimization_succes = false;
        PolicyBuilder builder;
        for (const auto& rule : current_policy.get_rules()) {
            std::string rule_repr = rule->compute_repr();
            if (finished_rule_reprs.count(rule_repr)) {
                continue;
            }
            for (const auto& condition : rule->get_conditions()) {
                std::string condition_repr = condition->compute_repr();
                if (condition_repr)
                builder.add_rule(compute_merged_values(rule->get_conditions(), {condition}, builder), compute_merged_values(rule->get_effects(), {}, builder));
                for (const auto& rule_2 : current_policy.get_rules()) {
                    if (rule == rule_2) {
                        continue;
                    }
                    rule_2->visit(builder);
                }
                Policy tmp_policy = builder.get_result();
                if (check_policy_matches_classification(tmp_policy, true_state_pairs, false_state_pairs)) {
                    minimization_succes = true;
                    current_policy = tmp_policy;
                    break;
                }
                rule_repr_to_finished_value_reprs[rule_repr].insert(condition_repr);
            }
            if (minimization_succes) break;
            for (const auto& effect : rule->get_effects()) {
                std::string effect_repr = effect->compute_repr();
                rule_repr_to_finished_value_reprs[rule_repr].insert(effect_repr);
            }
            if (minimization_succes) break;
            finished_rule_reprs.insert(rule_repr);
        }
    } while (true);
    */
    throw std::runtime_error("Not implemented.");
}

}