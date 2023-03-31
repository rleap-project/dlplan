#include "condition.h"
#include "effect.h"

#include "../utils/set_operators.h"
#include "../../include/dlplan/utils/hashing.h"

#include "../../include/dlplan/policy.h"

namespace dlplan::policy {
template<int size>
struct CacheEntry;
}

namespace std {
    template<int size>
    struct hash<dlplan::policy::CacheEntry<size>> {
        size_t operator()(const dlplan::policy::CacheEntry<size>& entry) const noexcept {
            size_t seed = entry.rules.size();
            dlplan::utils::hash_combine(seed, entry.feature);
            for (const auto& rule_ptr : entry.rules) {
                dlplan::utils::hash_combine(seed, rule_ptr);
            }
            return seed;
        }
    };
}

namespace dlplan::policy {
/**
 * A CacheEntry represents a set of rules that can be simplified with a given feature.
*/
template<int size>
struct CacheEntry {
    const dlplan::core::BaseElement* feature;
    std::array<const Rule*, size> rules;

    CacheEntry(const dlplan::core::BaseElement* feature, std::array<const Rule*, size>&& rules)
        : feature(feature), rules(std::move(rules)) { }

    bool operator==(const CacheEntry& other) const {
        if (this != &other) {
            return (feature == other.feature && rules == other.rules);
        }
        return true;
    }

    bool operator!=(const CacheEntry& other) const {
        return !(*this == other);
    }
};


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


static std::unordered_set<const Rule*> compute_dominated_rules(
    const std::unordered_set<const Rule*>& rules) {
    std::unordered_set<const Rule*> dominated_rules;
    for (const auto& rule_1 : rules) {
        for (const auto& rule_2 : rules) {
            if (rule_1 == rule_2) {
                // Note: there cannot be identical rules in a policy, hence this equality check suffices to not remove all identical rules.
                continue;
            }
            if (utils::is_subset_eq(rule_1->get_conditions(), rule_2->get_conditions()) && utils::is_subset_eq(rule_1->get_effects(), rule_2->get_effects())) {
                dominated_rules.insert(rule_2);
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

static void insert_rules_to_mapping(
    const Rule* rule,
    std::unordered_map<const BaseCondition*, std::vector<const Rule*>>& c2r,
    std::unordered_map<const BaseEffect*, std::vector<const Rule*>>& e2r) {
    for (const auto& condition : rule->get_conditions()) {
        c2r[condition.get()].push_back(rule);
    }
    for (const auto& effect : rule->get_effects()) {
        e2r[effect.get()].push_back(rule);
    }
}

static bool try_merge_boolean_condition(
    const Booleans& booleans,
    PolicyBuilder& builder,
    Rules& rules,
    std::unordered_set<const Rule*>& rules_result,
    std::unordered_map<const BaseCondition*, std::vector<const Rule*>>& c2r,
    std::unordered_map<const BaseEffect*, std::vector<const Rule*>>& e2r) {
    for (const auto& boolean : booleans) {
        const auto c_pos = builder.add_pos_condition(boolean);
        const auto c_neg = builder.add_neg_condition(boolean);
        for (const auto& rule1 : c2r[c_pos.get()]) {
            for (const auto& rule2 : c2r[c_neg.get()]) {
                // check mergeable
                if (!utils::is_subset_eq(rule1->get_effects(), rule2->get_effects())) continue;
                if (!utils::is_subset_eq(utils::set_difference(rule1->get_conditions(), {c_pos}), utils::set_difference(rule2->get_conditions(), {c_neg}))) continue;
                // merge
                std::shared_ptr<const Rule> result_rule = builder.add_rule(
                    utils::set_difference(rule2->get_conditions(), {c_neg}),
                    Effects(rule2->get_effects()));
                // was merged before
                if (rules.count(result_rule)) continue;
                // compute result
                rules.insert(result_rule);

                rules_result.erase(rule2);
                rules_result.insert(result_rule.get());
                insert_rules_to_mapping(result_rule.get(), c2r, e2r);
                /*
                std::cout << "try_merge_boolean_condition" << std::endl;
                std::cout << "rule1: " << rule1->str() << std::endl;
                std::cout << "rule2: " << rule2->str() << std::endl;
                std::cout << "rule_result: " << result_rule->str() << std::endl;
                */
                return true;
            }
        }
    }
    return false;
}

static bool try_merge_numerical_condition(
    const Numericals& numericals,
    PolicyBuilder& builder,
    Rules& rules,
    std::unordered_set<const Rule*>& rules_result,
    std::unordered_map<const BaseCondition*, std::vector<const Rule*>>& c2r,
    std::unordered_map<const BaseEffect*, std::vector<const Rule*>>& e2r) {
    for (const auto& numerical : numericals) {
        const auto c_gt = builder.add_gt_condition(numerical);
        const auto c_eq = builder.add_eq_condition(numerical);
        for (const auto& rule1 : c2r[c_gt.get()]) {
            for (const auto& rule2 : c2r[c_eq.get()]) {
                // check mergeable
                if (!utils::is_subset_eq(rule1->get_effects(), rule2->get_effects())) continue;
                if (!utils::is_subset_eq(utils::set_difference(rule1->get_conditions(), {c_gt}), utils::set_difference(rule2->get_conditions(), {c_eq}))) continue;
                // merge
                std::shared_ptr<const Rule> result_rule = builder.add_rule(
                    utils::set_difference(rule2->get_conditions(), {c_eq}),
                    Effects(rule2->get_effects()));
                // was merged before
                if (rules.count(result_rule)) continue;
                // compute result
                rules.insert(result_rule);
                rules_result.erase(rule2);
                rules_result.insert(result_rule.get());
                insert_rules_to_mapping(result_rule.get(), c2r, e2r);
                /*
                std::cout << "try_merge_numerical_condition" << std::endl;
                std::cout << "rule1: " << rule1->str() << std::endl;
                std::cout << "rule2: " << rule2->str() << std::endl;
                std::cout << "rule_result: " << result_rule->str() << std::endl;
                */
                return true;
            }
        }
    }
    return false;
}

static bool try_merge_boolean_effect(
    const Booleans& booleans,
    PolicyBuilder& builder,
    Rules& rules,
    std::unordered_set<const Rule*>& rules_result,
    std::unordered_map<const BaseCondition*, std::vector<const Rule*>>& c2r,
    std::unordered_map<const BaseEffect*, std::vector<const Rule*>>& e2r) {
    for (const auto& boolean : booleans) {
        const auto e_pos = builder.add_pos_effect(boolean);
        const auto e_neg = builder.add_neg_effect(boolean);
        const auto e_bot = builder.add_bot_effect(boolean);
        for (const auto& rule1 : e2r[e_pos.get()]) {
            for (const auto& rule2 : e2r[e_neg.get()]) {
                // check mergeable
                if (!utils::is_subset_eq(rule1->get_conditions(), rule2->get_conditions())) continue;
                if (!utils::is_subset_eq(utils::set_difference(rule1->get_effects(), {e_pos}), utils::set_difference(rule2->get_effects(), {e_neg}))) continue;
                // merge
                std::shared_ptr<const Rule> result_rule = builder.add_rule(
                    Conditions(rule2->get_conditions()),
                    utils::set_difference(rule2->get_effects(), {e_neg}));
                // was merged before
                if (rules.count(result_rule)) continue;
                // compute result
                rules.insert(result_rule);

                rules_result.erase(rule2);
                rules_result.insert(result_rule.get());
                insert_rules_to_mapping(result_rule.get(), c2r, e2r);
                /*
                std::cout << "try_merge_boolean_effect" << std::endl;
                std::cout << "rule1: " << rule1->str() << std::endl;
                std::cout << "rule2: " << rule2->str() << std::endl;
                std::cout << "rule_result: " << result_rule->str() << std::endl;
                */
                return true;
            }
        }
        for (const auto& rule1 : e2r[e_pos.get()]) {
            for (const auto& rule2 : e2r[e_bot.get()]) {
                // check mergeable
                if (!utils::is_subset_eq(rule1->get_conditions(), rule2->get_conditions())) continue;
                if (!utils::is_subset_eq(utils::set_difference(rule1->get_effects(), {e_pos}), utils::set_difference(rule2->get_effects(), {e_bot}))) continue;
                // merge
                std::shared_ptr<const Rule> result_rule = builder.add_rule(
                    Conditions(rule2->get_conditions()),
                    utils::set_difference(rule2->get_effects(), {e_bot}));
                // was merged before
                if (rules.count(result_rule)) continue;
                // compute result
                rules.insert(result_rule);

                rules_result.erase(rule2);
                rules_result.insert(result_rule.get());
                insert_rules_to_mapping(result_rule.get(), c2r, e2r);
                /*
                std::cout << "try_merge_boolean_effect" << std::endl;
                std::cout << "rule1: " << rule1->str() << std::endl;
                std::cout << "rule2: " << rule2->str() << std::endl;
                std::cout << "rule_result: " << result_rule->str() << std::endl;
                */
                return true;
            }
        }
        for (const auto& rule1 : e2r[e_neg.get()]) {
            for (const auto& rule2 : e2r[e_bot.get()]) {
                CacheEntry<2> key(boolean.get(), {rule1, rule2});
                // check mergeable
                if (!utils::is_subset_eq(rule1->get_conditions(), rule2->get_conditions())) continue;
                if (!utils::is_subset_eq(utils::set_difference(rule1->get_effects(), {e_neg}), utils::set_difference(rule2->get_effects(), {e_bot}))) continue;
                // merge
                std::shared_ptr<const Rule> result_rule = builder.add_rule(
                    Conditions(rule2->get_conditions()),
                    utils::set_difference(rule2->get_effects(), {e_bot}));
                // was merged before
                if (rules.count(result_rule)) continue;
                // compute result
                rules.insert(result_rule);

                rules_result.erase(rule2);
                rules_result.insert(result_rule.get());
                insert_rules_to_mapping(result_rule.get(), c2r, e2r);
                /*
                std::cout << "try_merge_boolean_effect" << std::endl;
                std::cout << "rule1: " << rule1->str() << std::endl;
                std::cout << "rule2: " << rule2->str() << std::endl;
                std::cout << "rule_result: " << result_rule->str() << std::endl;
                */
                return true;
            }
        }
    }
    return false;
}

static bool try_merge_numerical_effect(
    const Numericals& numericals,
    PolicyBuilder& builder,
    Rules& rules,
    std::unordered_set<const Rule*>& rules_result,
    std::unordered_map<const BaseCondition*, std::vector<const Rule*>>& c2r,
    std::unordered_map<const BaseEffect*, std::vector<const Rule*>>& e2r) {
    for (const auto& numerical : numericals) {
        const auto e_inc = builder.add_inc_effect(numerical);
        const auto e_dec = builder.add_dec_effect(numerical);
        const auto e_bot = builder.add_bot_effect(numerical);
        for (const auto& rule1 : e2r[e_inc.get()]) {
            for (const auto& rule2 : e2r[e_dec.get()]) {
                // check mergeable
                if (!utils::is_supset_eq(rule1->get_conditions(), rule2->get_conditions())) continue;
                if (!utils::is_supset_eq(utils::set_difference(rule1->get_effects(), {e_inc}), utils::set_difference(rule2->get_effects(), {e_dec}))) continue;
                for (const auto& rule3 : e2r[e_bot.get()]) {
                    // check mergeable
                    if (!utils::is_supset_eq(rule1->get_conditions(), rule3->get_conditions())) continue;
                    if (!utils::is_supset_eq(utils::set_difference(rule1->get_effects(), {e_inc}), utils::set_difference(rule3->get_effects(), {e_bot}))) continue;
                    // merge
                    std::shared_ptr<const Rule> result_rule = builder.add_rule(
                        Conditions(rule1->get_conditions()),
                        utils::set_difference(rule1->get_effects(), {e_inc}));
                    // was merged before
                    if (rules.count(result_rule)) continue;
                    // compute result
                    rules.insert(result_rule);
                    rules_result.erase(rule1);
                    rules_result.insert(result_rule.get());
                    insert_rules_to_mapping(result_rule.get(), c2r, e2r);
                    /*
                    std::cout << "try_merge_numerical_effect" << std::endl;
                    std::cout << "rule1: " << rule1->str() << std::endl;
                    std::cout << "rule2: " << rule2->str() << std::endl;
                    std::cout << "rule3: " << rule3->str() << std::endl;
                    std::cout << "rule_result: " << result_rule->str() << std::endl;
                    */
                    return true;
                }
            }
        }
    }
    return false;
}

Policy PolicyMinimizer::minimize(const Policy& policy) const {
    // successively add simpler rules that are made up of existing rules
    PolicyBuilder builder;
    Policy tmp_policy = policy.copy_to_builder(builder);
    Rules rules = tmp_policy.get_rules();
    std::unordered_set<const Rule*> rules_result;
    std::for_each(rules.begin(), rules.end(), [&](const auto& rule){ rules_result.insert(rule.get()); });
    Booleans booleans = builder.get_booleans();
    Numericals numericals = builder.get_numericals();
    std::unordered_map<const BaseCondition*, std::vector<const Rule*>> c2r;
    std::unordered_map<const BaseEffect*, std::vector<const Rule*>> e2r;
    for (const auto& rule : rules) {
        insert_rules_to_mapping(rule.get(), c2r, e2r);
    }
    do {
        if (try_merge_boolean_condition(booleans, builder, rules, rules_result, c2r, e2r)) continue;
        if (try_merge_numerical_condition(numericals, builder, rules, rules_result, c2r, e2r)) continue;
        if (try_merge_boolean_effect(booleans, builder, rules, rules_result, c2r, e2r)) continue;
        if (try_merge_numerical_effect(numericals, builder, rules, rules_result, c2r, e2r)) continue;
        break;
    } while(true);
    // Erase dominated rules
    for (const auto& rule : compute_dominated_rules(rules_result)) {
        rules_result.erase(rule);
    }
    // Copy resulting rules in new builder and return policy.
    PolicyBuilder result_builder;
    std::for_each(rules.begin(), rules.end(), [&](const auto& rule){
        if (rules_result.count(rule.get())) {
            rule->copy_to_builder(result_builder);
        }
    });
    return result_builder.get_result();
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
            for (const auto& effect : rule->get_effects()) {
                PolicyBuilder builder;
                builder.add_rule(
                    copy_to_builder(rule->get_conditions(), builder),
                    copy_to_builder(utils::set_difference(rule->get_effects(), {effect}), builder));
                Rules rules = copy_to_builder(utils::set_difference(current_policy.get_rules(), {rule}), builder);
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
    } while (minimization_success);
    return current_policy;
}

}