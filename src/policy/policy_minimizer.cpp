#include "condition.h"
#include "effect.h"

#include "../utils/set_operators.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {

/**
 *
 */
template<typename T>
std::vector<T> copy_values_to_builder(
    const std::vector<T>& old_values,
    PolicyBuilder& builder) {
    std::vector<T> new_values;
    new_values.reserve(old_values.size());
    for (const auto& element : old_values) {
        new_values.push_back(element->visit(builder));
    }
    return new_values;
}


/**
 * Returns true iff all values of given type PARENT_T are of type SUB_T
 */
template<typename PARENT_T, typename SUB_T>
static bool check_subtype_equality(
    const std::vector<std::shared_ptr<const PARENT_T>>& values) {
    return std::all_of(values.begin(), values.end(), [](const std::shared_ptr<const PARENT_T>& value){ return std::dynamic_pointer_cast<const SUB_T>(value); });
}


/**
 * Returns true iff all values of given type T have feature with same index.
 */
template<typename T>
static bool check_feature_index_equality(
    const std::vector<std::shared_ptr<const T>>& values) {
    if (values.empty()) return true;
    return std::all_of(values.begin(), values.end(), [index=(*(values.begin()))->get_base_feature()->get_index()](const std::shared_ptr<const T>& value){ return value->get_base_feature()->get_index() == index; } );
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
            std::vector<std::shared_ptr<const BaseCondition>> symmetric_diff = utils::set_symmetric_difference<std::shared_ptr<const BaseCondition>>({rule_1->get_conditions(), rule_2->get_conditions()});
            if (symmetric_diff.empty()) {
                continue;
            }
            if (!check_feature_index_equality(symmetric_diff) ||
                !(check_subtype_equality<BaseCondition, BooleanCondition>(symmetric_diff) || check_subtype_equality<BaseCondition, NumericalCondition>(symmetric_diff))) {
                continue;
            }
            const auto rule = builder.add_rule(
                utils::set_difference(copy_values_to_builder(rule_1->get_conditions(), builder), copy_values_to_builder(symmetric_diff, builder)),
                copy_values_to_builder(rule_1->get_effects(), builder));
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
            std::vector<std::shared_ptr<const BaseEffect>> symmetric_diff = utils::set_symmetric_difference<std::shared_ptr<const BaseEffect>>({rule_1->get_effects(), rule_2->get_effects()});
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
                symmetric_diff = utils::set_symmetric_difference<std::shared_ptr<const BaseEffect>>({rule_1->get_effects(), rule_2->get_effects(), rule_3->get_effects()});
                if (symmetric_diff.empty()) {
                    continue;
                }
                if (!check_feature_index_equality(symmetric_diff) ||
                    !(check_subtype_equality<BaseEffect, BooleanEffect>(symmetric_diff) || check_subtype_equality<BaseEffect, NumericalEffect>(symmetric_diff))) {
                    continue;
                }
                const auto rule = builder.add_rule(
                    copy_values_to_builder(rule_1->get_conditions(), builder),
                    utils::set_difference(copy_values_to_builder(rule_1->get_effects(), builder), copy_values_to_builder(symmetric_diff, builder)));
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
    // TODO: avoid rechecking merges.
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
    // untested naive version.
    // TODO: avoid rechecking conditions
    Policy current_policy = policy;
    bool minimization_success;
    do {
        minimization_success = false;
        for (const auto& rule_1 : current_policy.get_rules()) {
            for (const auto& condition : rule_1->get_conditions()) {
                PolicyBuilder builder;
                builder.add_rule(utils::set_difference(rule_1->get_conditions(), {condition}), rule_1->get_effects());
                for (const auto& rule_2 : current_policy.get_rules()) {
                    if (rule_1 == rule_2) {
                        continue;
                    }
                    rule_2->visit(builder);
                }
                Policy tmp_policy = builder.get_result();
                if (check_policy_matches_classification(tmp_policy, true_state_pairs, false_state_pairs)) {
                    minimization_success = true;
                    current_policy = tmp_policy;
                    break;
                }
            }
            if (minimization_success) {
                break;
            }
            for (const auto& effect : rule_1->get_effects()) {
                PolicyBuilder builder;
                builder.add_rule(rule_1->get_conditions(), utils::set_difference(rule_1->get_effects(), {effect}));
                for (const auto& rule_2 : current_policy.get_rules()) {
                    if (rule_1 == rule_2) {
                        continue;
                    }
                    rule_2->visit(builder);
                }
                Policy tmp_policy = builder.get_result();
                if (check_policy_matches_classification(tmp_policy, true_state_pairs, false_state_pairs)) {
                    minimization_success = true;
                    current_policy = tmp_policy;
                    break;
                }
            }
            if (minimization_success) {
                break;
            }
        }
    } while (!minimization_success);
    return current_policy;
}

}