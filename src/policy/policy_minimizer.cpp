#include "condition.h"
#include "effect.h"

#include "../utils/set_operators.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {

/**
 * Adds values to the builder with original_values minus removed_values and returns them.
 */
template<typename T>
static std::vector<std::shared_ptr<const T>> compute_merged_values(
    const std::vector<std::shared_ptr<const T>>& original_values,
    const std::unordered_set<std::shared_ptr<const T>>& removed_values,
    PolicyBuilder& builder) {
    std::vector<std::shared_ptr<const T>> values;
    for (const auto& value : original_values) {
        if (removed_values.count(value)) {
            continue;
        }
        values.push_back(value->visit(builder));
    }
    return values;
}


/**
 * Returns true iff all values of given type PARENT_T are of type SUB_T
 */
template<typename PARENT_T, typename SUB_T>
static bool check_subtype_equality(
    const std::unordered_set<std::shared_ptr<const PARENT_T>>& values) {
    return std::all_of(values.begin(), values.end(), [](const std::shared_ptr<const PARENT_T>& value){ return std::dynamic_pointer_cast<const SUB_T>(value); });
}


/**
 * Returns true iff all values of given type T have feature with same index.
 */
template<typename T>
static bool check_feature_index_equality(
    const std::unordered_set<std::shared_ptr<const T>>& values) {
    if (values.empty()) return true;
    return std::all_of(values.begin(), values.end(), [index=(*(values.begin()))->get_base_feature()->get_index()](const std::shared_ptr<const T>& value){ return value->get_base_feature()->get_index() == index; } );
}


/**
 * Computes the symmetric difference of the given values of all rules.
 */
template<typename PARENT_T>
static std::unordered_set<std::shared_ptr<const PARENT_T>> compute_symmetric_difference(
    const std::vector<std::vector<std::shared_ptr<const PARENT_T>>>& values_by_rule) {
    std::unordered_map<std::shared_ptr<const PARENT_T>, int> value_frequencies;
    for (const auto& values : values_by_rule) {
        for (const auto& v : values) {
            if (value_frequencies.count(v)) {
                ++value_frequencies[v];
            } else {
                value_frequencies[v] = 1;
            }
        }
    }
    std::unordered_set<std::shared_ptr<const PARENT_T>> symmetric_diff;
    for (const auto& p : value_frequencies) {
        if (value_frequencies[p.first] == 1) {
            symmetric_diff.insert(p.first);
        }
    }
    return symmetric_diff;
}


static std::unordered_set<std::shared_ptr<const Rule>> try_merge_by_condition(
    const Policy& policy, PolicyBuilder& builder) {
    for (const auto& rule_1 : policy.get_rules()) {
        for (const auto& rule_2 : policy.get_rules()) {
            if (rule_1->get_index() >= rule_2->get_index()) {
                continue;
            }
            if (rule_1->get_effects() != rule_2->get_effects()) {
                continue;
            }
            std::unordered_set<std::shared_ptr<const BaseCondition>> symmetric_diff = compute_symmetric_difference<BaseCondition>({rule_1->get_conditions(), rule_2->get_conditions()});
            if (symmetric_diff.empty()) {
                continue;
            }
            if (!check_feature_index_equality(symmetric_diff) ||
                !(check_subtype_equality<BaseCondition, BooleanCondition>(symmetric_diff) || check_subtype_equality<BaseCondition, NumericalCondition>(symmetric_diff))) {
                continue;
            }
            builder.add_rule(compute_merged_values(rule_1->get_conditions(), symmetric_diff, builder), compute_merged_values(rule_1->get_effects(), {}, builder));
            return {rule_1, rule_2};
        }
    }
    return {};
}


static std::unordered_set<std::shared_ptr<const Rule>> try_merge_by_effect(
    const Policy& policy, PolicyBuilder& builder) {
    for (const auto& rule_1 : policy.get_rules()) {
        for (const auto& rule_2 : policy.get_rules()) {
            if (rule_1->get_index() >= rule_2->get_index()) {
                continue;
            }
            if (rule_1->get_conditions() != rule_2->get_conditions()) {
                continue;
            }
            std::unordered_set<std::shared_ptr<const BaseEffect>> symmetric_diff = compute_symmetric_difference<BaseEffect>({rule_1->get_effects(), rule_2->get_effects()});
            if (symmetric_diff.empty()) {
                continue;
            }
            if (!check_feature_index_equality(symmetric_diff) ||
                !(check_subtype_equality<BaseEffect, BooleanEffect>(symmetric_diff) || check_subtype_equality<BaseEffect, NumericalEffect>(symmetric_diff))) {
                continue;
            }
            for (const auto& rule_3 : policy.get_rules()) {
                if (rule_2->get_index() >= rule_3->get_index()) {
                    continue;
                }
                if (rule_2->get_conditions() != rule_3->get_conditions()) {
                    continue;
                }
                symmetric_diff = compute_symmetric_difference<BaseEffect>({rule_1->get_effects(), rule_2->get_effects(), rule_3->get_effects()});
                if (symmetric_diff.empty()) {
                    continue;
                }
                if (!check_feature_index_equality(symmetric_diff) ||
                    !(check_subtype_equality<BaseEffect, BooleanEffect>(symmetric_diff) || check_subtype_equality<BaseEffect, NumericalEffect>(symmetric_diff))) {
                    continue;
                }
                builder.add_rule(compute_merged_values(rule_1->get_conditions(), {}, builder), compute_merged_values(rule_1->get_effects(), symmetric_diff, builder));
                return {rule_1, rule_2, rule_3};
            }
        }
    }
    return {};
}


static std::unordered_set<std::shared_ptr<const Rule>> compute_dominated_rules(
    const Policy& policy) {
    std::unordered_set<std::shared_ptr<const Rule>> dominated_rules;
    for (const auto& rule_1 : policy.get_rules()) {
        for (const auto& rule_2 : policy.get_rules()) {
            if (rule_1 == rule_2) {
                // Note: there cannot be identical rules in a policy, hence this equality check suffices to not remove all identical rules.
                continue;
            }
            if (utils::is_subset_eq(rule_1->get_conditions(), rule_2->get_conditions()) && utils::is_subset_eq(rule_1->get_effects(), rule_2->get_effects())) {
                dominated_rules.insert(rule_2);
                break;
            }
        }
    }
    return dominated_rules;
}


/**
 * Returns true iff policy classifies true_state_pairs as true and false_state_pairs as false.
 */
static bool check_policy_matches_classification(
    const Policy& policy,
    const core::StatePairs& true_state_pairs,
    const core::StatePairs& false_state_pairs) {
    return std::all_of(true_state_pairs.begin(), true_state_pairs.end(), [&policy](const core::StatePair& state_pair){ return policy.evaluate_lazy(state_pair.first, state_pair.second); }) &&
           std::all_of(false_state_pairs.begin(), false_state_pairs.end(), [&policy](const core::StatePair& state_pair){ return !policy.evaluate_lazy(state_pair.first, state_pair.second); });
}


PolicyMinimizer::PolicyMinimizer() { }

PolicyMinimizer::PolicyMinimizer(const PolicyMinimizer& other) = default;

PolicyMinimizer& PolicyMinimizer::operator=(const PolicyMinimizer& other) = default;

PolicyMinimizer::PolicyMinimizer(PolicyMinimizer&& other) = default;

PolicyMinimizer& PolicyMinimizer::operator=(PolicyMinimizer&& other) = default;

PolicyMinimizer::~PolicyMinimizer() { }


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
    std::unordered_set<std::shared_ptr<const Rule>> dominated_rules = compute_dominated_rules(current_policy);
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