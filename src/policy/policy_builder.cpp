#include "policy_builder.h"

#include "condition.h"
#include "effect.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {

std::shared_ptr<const core::Boolean> PolicyBuilderImpl::add_boolean_feature(core::Boolean boolean) {
    auto result = m_caches.m_boolean_cache->insert(std::make_unique<core::Boolean>(boolean));
    if (result.second) {
        m_boolean_features.push_back(result.first);
    }
    return result.first;
}

std::shared_ptr<const core::Numerical> PolicyBuilderImpl::add_numerical_feature(core::Numerical numerical) {
    auto result = m_caches.m_numerical_cache->insert(std::make_unique<core::Numerical>(numerical));
    if (result.second) {
        m_numerical_features.push_back(result.first);
    }
    return result.first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_pos_condition(std::shared_ptr<const core::Boolean> b) {
    return m_caches.m_condition_cache->insert(std::make_unique<PositiveBooleanCondition>(PositiveBooleanCondition(b))).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_neg_condition(std::shared_ptr<const core::Boolean> b) {
    return m_caches.m_condition_cache->insert(std::make_unique<NegativeBooleanCondition>(NegativeBooleanCondition(b))).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_gt_condition(std::shared_ptr<const core::Numerical> n) {
    return m_caches.m_condition_cache->insert(std::make_unique<GreaterNumericalCondition>(GreaterNumericalCondition(n))).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_eq_condition(std::shared_ptr<const core::Numerical> n) {
    return m_caches.m_condition_cache->insert(std::make_unique<EqualNumericalCondition>(EqualNumericalCondition(n))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_pos_effect(std::shared_ptr<const core::Boolean> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<PositiveBooleanEffect>(PositiveBooleanEffect(b))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_neg_effect(std::shared_ptr<const core::Boolean> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<NegativeBooleanEffect>(NegativeBooleanEffect(b))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_bot_effect(std::shared_ptr<const core::Boolean> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedBooleanEffect>(UnchangedBooleanEffect(b))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_inc_effect(std::shared_ptr<const core::Numerical> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<IncrementNumericalEffect>(IncrementNumericalEffect(n))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_dec_effect(std::shared_ptr<const core::Numerical> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<DecrementNumericalEffect>(DecrementNumericalEffect(n))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_bot_effect(std::shared_ptr<const core::Numerical> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedNumericalEffect>(UnchangedNumericalEffect(n))).first;
}

std::shared_ptr<const Rule> PolicyBuilderImpl::add_rule(
    std::vector<std::shared_ptr<const BaseCondition>>&& conditions,
    std::vector<std::shared_ptr<const BaseEffect>>&& effects) {
    // do not add rules that are a dominated by other rules.
    std::unordered_set<std::shared_ptr<const BaseCondition>> conditions_set(conditions.begin(), conditions.end());
    std::unordered_set<std::shared_ptr<const BaseEffect>> effects_set(effects.begin(), effects.end());
    for (const auto& rule : m_rules) {
        bool dominates = true;
        // TODO: implement general function to compute subset.
        for (const auto& condition : rule->get_conditions()) {
            if (!conditions_set.count(condition)) {
                dominates = false;
                break;
            }
        }
        if (dominates) {
            for (const auto& effect : rule->get_effects()) {
                if (!effects_set.count(effect)) {
                    dominates = false;
                    break;
                }
            }
        }
        if (dominates) {
            return rule;
        }
    }
    auto result = m_caches.m_rule_cache->insert(std::make_unique<Rule>(Rule(std::move(conditions), std::move(effects), m_rules.size())));
    if (result.second) {
        m_rules.push_back(result.first);
    }
    return result.first;
}

Policy PolicyBuilderImpl::get_result() {
    return Policy(m_boolean_features, m_numerical_features, m_rules);
}

}
