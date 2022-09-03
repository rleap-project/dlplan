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
template std::vector<std::shared_ptr<const BaseCondition>> compute_merged_values(
    const std::vector<std::shared_ptr<const BaseCondition>>&,
    const std::unordered_set<std::shared_ptr<const BaseCondition>>&,
    PolicyBuilder&);
template std::vector<std::shared_ptr<const BaseEffect>> compute_merged_values(
    const std::vector<std::shared_ptr<const BaseEffect>>&,
    const std::unordered_set<std::shared_ptr<const BaseEffect>>&,
    PolicyBuilder&);


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
 * Returns merge compatible values or the empty set if none exist.
 * Merge compatible values must have same feature index,
 * all subtypes are either SUB_T1 or SUB_T2.
 */
template<typename PARENT_T, typename SUB_T1, typename SUB_T2>
static std::unordered_set<std::shared_ptr<const PARENT_T>> compute_mergeable_values(
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
    if (!check_feature_index_equality(symmetric_diff) ||
        !(check_subtype_equality<PARENT_T, SUB_T1>(symmetric_diff) || check_subtype_equality<PARENT_T, SUB_T2>(symmetric_diff))) {
        return {};
    }
    return symmetric_diff;
}
template std::unordered_set<std::shared_ptr<const BaseCondition>> compute_mergeable_values<BaseCondition, BooleanCondition, NumericalCondition>(
    const std::vector<std::vector<std::shared_ptr<const BaseCondition>>>&);
template std::unordered_set<std::shared_ptr<const BaseEffect>> compute_mergeable_values<BaseEffect, BooleanEffect, NumericalEffect>(
    const std::vector<std::vector<std::shared_ptr<const BaseEffect>>>&);


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


static std::unordered_set<std::shared_ptr<const Rule>> compute_dominated_rules(
    const Policy& policy) {
    std::unordered_set<std::shared_ptr<const Rule>> dominated_rules;
    for (const auto& rule_1 : policy.get_rules()) {
        auto rule_1_conditions = rule_1->get_conditions();
        auto rule_1_effects = rule_1->get_effects();
        std::unordered_set<std::shared_ptr<const BaseCondition>> rule_1_conditions_set(rule_1_conditions.begin(), rule_1_conditions.end());
        std::unordered_set<std::shared_ptr<const BaseEffect>> rule_1_effects_set(rule_1_effects.begin(), rule_1_effects.end());
        for (const auto& rule_2 : policy.get_rules()) {
            if (rule_1 == rule_2) {
                // Note: there cannot be identical rules in a policy, hence this equality check suffices to not remove all identical rules.
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
    return dominated_rules;
}


/**
 * Returns true iff policy classifies true_state_pairs as true and false_state_pairs as false.
 */
static bool check_policy_matches_classification(
    const Policy& policy,
    const core::StatePairs& true_state_pairs,
    const core::StatePairs& false_state_pairs) {
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