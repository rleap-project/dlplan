#include "condition.h"
#include "effect.h"

#include "../utils/set_operators.h"
#include "../../include/dlplan/utils/hashing.h"

#include "../../include/dlplan/policy.h"

namespace std {
    template<> struct hash<std::vector<const dlplan::policy::Rule*>> {
        size_t operator()(const std::vector<const dlplan::policy::Rule*>& merged_rule_combinations) const noexcept {
            size_t seed = merged_rule_combinations.size();
            for (const auto& rule_ptr : merged_rule_combinations) {
                dlplan::utils::hash_combine(seed, rule_ptr);
            }
            return seed;
        }
    };
}

namespace dlplan::policy {
/**
 * Copies all objects to the given PolicyBuilder and returns newly constructed objects.
 */
template<typename T>
std::set<T> copy_to_builder(
    const std::set<T>& old_objects,
    PolicyBuilder& builder) {
    std::set<T> new_objects;
    std::transform(
        old_objects.begin(),
        old_objects.end(),
        std::inserter(new_objects, new_objects.begin()),
        [&builder](const auto& object){
            return object->copy_to_builder(builder);
        }
    );
    return new_objects;
}


template<typename T>
static bool check_feature_equality(
    const std::set<std::shared_ptr<const T>>& objects) {
    if (objects.empty()) return true;
    return std::all_of(
        objects.begin(),
        objects.end(),
        [feature=(*(objects.begin()))->get_base_feature()](
            const std::shared_ptr<const T>& object){
                return object->get_base_feature() == feature;
            }
        );
}


template<typename T, typename T_SUB>
static bool check_object_type(
    const std::set<std::shared_ptr<const T>>& objects) {
    if (objects.empty()) return true;
    return std::all_of(
        objects.begin(),
        objects.end(),
        [](const std::shared_ptr<const T>& object){
            return std::dynamic_pointer_cast<const T_SUB>(object) != nullptr;
        }
    );
}


static void try_merge_by_condition(
    PolicyBuilder& builder,
    Rules& rules,
    std::unordered_set<std::vector<const Rule*>>& merged_rule_combinations,
    Rules& merged_rules) {
    Rules added_rules;
    for (const auto& rule_1 : rules) {
        for (const auto& rule_2 : rules) {
            auto merge = std::vector<const Rule*>({rule_1.get(), rule_2.get()});
            if (merged_rule_combinations.count(merge)) {
                continue;
            }
            if (rule_1->get_index() >= rule_2->get_index()) {
                continue;
            }
            if (rule_1->get_effects() != rule_2->get_effects()) {
                continue;
            }
            // check that effects are compatible to merge
            Conditions symmetric_diff = utils::set_symmetric_difference(rule_1->get_conditions(), rule_2->get_conditions());
            if (symmetric_diff.size() != 2) {
                continue;
            }
            if (!check_feature_equality(symmetric_diff)) {
                continue;
            }
            if  (!check_object_type<BaseCondition, BooleanCondition>(symmetric_diff) &&
                 !check_object_type<BaseCondition, NumericalCondition>(symmetric_diff)) {
                continue;
            }
            // check that other conditions are identical
            Conditions rule_1_other_conditions = utils::set_difference(rule_1->get_conditions(), symmetric_diff);
            Conditions rule_2_other_conditions = utils::set_difference(rule_2->get_conditions(), symmetric_diff);
            if (!(rule_1_other_conditions == rule_2_other_conditions)) {
                continue;
            }
            added_rules.insert(builder.add_rule(
                copy_to_builder(rule_1_other_conditions, builder),
                copy_to_builder(rule_1->get_effects(), builder)));
            merged_rule_combinations.insert(merge);
            merged_rules.insert({rule_1, rule_2});
        }
    }
    rules.insert(added_rules.begin(), added_rules.end());
}

static void try_merge_by_numerical_effect(
    PolicyBuilder& builder,
    Rules& rules,
    std::unordered_set<std::vector<const Rule*>>& merged_rule_combinations,
    Rules& merged_rules) {
    Rules added_rules;
    for (const auto& rule_1 : rules) {
        for (const auto& rule_2 : rules) {
            if (rule_1->get_index() >= rule_2->get_index()) {
                continue;
            }
            if (rule_1->get_conditions() != rule_2->get_conditions()) {
                continue;
            }

            Effects symmetric_diff = utils::set_symmetric_difference(rule_1->get_effects(), rule_2->get_effects());
            if (symmetric_diff.empty()) {
                continue;
            }
            if (!check_feature_equality(symmetric_diff)) {
                continue;
            }
            for (const auto& rule_3 : rules) {
                auto merge = std::vector<const Rule*>({rule_1.get(), rule_2.get(), rule_3.get()});
                if (merged_rule_combinations.count(merge)) {
                    continue;
                }
                if (rule_2->get_index() >= rule_3->get_index()) {
                    continue;
                }
                if (rule_2->get_conditions() != rule_3->get_conditions()) {
                    continue;
                }
                // check that effects are compatible to merge
                symmetric_diff = utils::set_symmetric_difference(rule_1->get_effects(), rule_2->get_effects());
                symmetric_diff = utils::set_difference(symmetric_diff, rule_3->get_effects());
                if (symmetric_diff.size() != 3) {
                    continue;
                }
                if (!check_feature_equality(symmetric_diff)) {
                    continue;
                }
                if  (!check_object_type<BaseEffect, NumericalEffect>(symmetric_diff)) {
                    continue;
                }
                // check that other effects are identical
                const auto rule_1_other_effects = utils::set_difference(rule_1->get_effects(), symmetric_diff);
                const auto rule_2_other_effects = utils::set_difference(rule_2->get_effects(), symmetric_diff);
                const auto rule_3_other_effects = utils::set_difference(rule_3->get_effects(), symmetric_diff);
                if (!(rule_1_other_effects == rule_2_other_effects) && (rule_1_other_effects == rule_3_other_effects)) {
                    continue;
                }
                added_rules.insert(builder.add_rule(
                    copy_to_builder(rule_1->get_conditions(), builder),
                    copy_to_builder(utils::set_difference(rule_1->get_effects(), symmetric_diff), builder)));
                merged_rule_combinations.insert(merge);
                merged_rules.insert({rule_1, rule_2, rule_3});
            }
        }
    }
    rules.insert(added_rules.begin(), added_rules.end());
}

static void
try_merge_by_boolean_effect(
    PolicyBuilder& builder,
    Rules& rules,
    std::unordered_set<std::vector<const Rule*>>& merged_rule_combinations,
    Rules& merged_rules) {
    Rules added_rules;
    for (const auto& rule_1 : rules) {
        for (const auto& rule_2 : rules) {
            auto merge = std::vector<const Rule*>({rule_1.get(), rule_2.get()});
            if (merged_rule_combinations.count(merge)) {
                continue;
            }
            if (rule_1->get_index() >= rule_2->get_index()) {
                continue;
            }
            if (rule_1->get_conditions() != rule_2->get_conditions()) {
                continue;
            }
            // check that effects are compatible to merge
            Effects symmetric_diff = utils::set_symmetric_difference(rule_1->get_effects(), rule_2->get_effects());
            if (symmetric_diff.size() != 2) {
                continue;
            }
            if (!check_feature_equality(symmetric_diff)) {
                continue;
            }
            if  (!check_object_type<BaseEffect, BooleanEffect>(symmetric_diff)) {
                continue;
            }
            // check that other effects are identical
            const auto rule_1_other_effects = utils::set_difference(rule_1->get_effects(), symmetric_diff);
            const auto rule_2_other_effects = utils::set_difference(rule_2->get_effects(), symmetric_diff);
            if (!(rule_1_other_effects == rule_2_other_effects)) {
                continue;
            }
            added_rules.insert(builder.add_rule(
                copy_to_builder(rule_1->get_conditions(), builder),
                copy_to_builder(utils::set_difference(rule_1->get_effects(), symmetric_diff), builder)));
            merged_rule_combinations.insert(merge);
            merged_rules.insert({rule_1, rule_2});
        }
    }
    rules.insert(added_rules.begin(), added_rules.end());
}


static std::set<std::shared_ptr<const Rule>> compute_dominated_rules(
    const std::set<std::shared_ptr<const Rule>>& rules) {
    std::set<std::shared_ptr<const Rule>> dominated_rules;
    for (const auto& rule_1 : rules) {
        for (const auto& rule_2 : rules) {
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
    // successively add simpler rules that are made up of existing rules
    PolicyBuilder builder;
    Policy tmp_policy = policy.copy_to_builder(builder);
    bool changed = false;
    do {

    } while(changed);
    // remove dominated rules and return policy.

    /*
    Rules rules;
    PolicyBuilder minimization_builder;
    // copy original rules to a fresh builder
    auto added_rules = copy_to_builder(policy.get_rules(), minimization_builder);
    rules.insert(added_rules.begin(), added_rules.end());
    // avoid merging same rules again.
    std::unordered_set<std::vector<const Rule*>> merged_rule_combinations;
    // collect rules that were merged
    Rules merged_rules;
    size_t old_size;
    do {
        old_size = rules.size();
        try_merge_by_condition(minimization_builder, rules, merged_rule_combinations, merged_rules);
        try_merge_by_numerical_effect(minimization_builder, rules, merged_rule_combinations, merged_rules);
        try_merge_by_boolean_effect(minimization_builder, rules, merged_rule_combinations, merged_rules);
    } while (rules.size() > old_size);
    // Remove merged rules
    rules = utils::set_difference(rules, merged_rules);
    // Remove dominated rule
    rules = utils::set_difference(rules, compute_dominated_rules(rules));
    PolicyBuilder result_builder;
    rules = copy_to_builder(rules, result_builder);
    auto result = result_builder.add_policy(move(rules));
    return std::move(*result.get());
    */
}

Policy PolicyMinimizer::minimize(const Policy& policy, const core::StatePairs& true_state_pairs, const core::StatePairs& false_state_pairs) const {
    Policy current_policy = policy;
    bool minimization_success;
    do {
        minimization_success = false;
        for (const auto& rule : current_policy.get_rules()) {
            for (const auto& condition : rule->get_conditions()) {
                PolicyBuilder builder;
                builder.add_rule(
                    copy_to_builder(utils::set_difference(rule->get_conditions(), {condition}), builder),
                    copy_to_builder(rule->get_effects(), builder));
                Rules rules = copy_to_builder(utils::set_difference(current_policy.get_rules(), {rule}), builder);
                Policy tmp_policy = *builder.add_policy(std::move(rules)).get();
                if (check_policy_matches_classification(tmp_policy, true_state_pairs, false_state_pairs)) {
                    minimization_success = true;
                    current_policy = tmp_policy;
                    break;
                }
            }
            if (minimization_success) {
                break;
            }
            for (const auto& effect : rule->get_effects()) {
                PolicyBuilder builder;
                builder.add_rule(
                    copy_to_builder(rule->get_conditions(), builder),
                    copy_to_builder(utils::set_difference(rule->get_effects(), {effect}), builder));
                Rules rules = copy_to_builder(utils::set_difference(current_policy.get_rules(), {rule}), builder);
                Policy tmp_policy = *builder.add_policy(std::move(rules)).get();
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
    } while (minimization_success);
    return current_policy;
}

}