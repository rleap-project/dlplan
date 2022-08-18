#include "policy_builder.h"

#include "condition.h"
#include "effect.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {

std::shared_ptr<const BooleanFeature> PolicyBuilderImpl::add_boolean_feature(core::Boolean boolean) {
    auto result = m_caches.m_boolean_cache->insert(std::make_unique<BooleanFeature>(BooleanFeature(m_boolean_features.size(), std::move(boolean))));
    if (result.second) {
        m_boolean_features.push_back(result.first);
    }
    return result.first;
}

std::shared_ptr<const NumericalFeature> PolicyBuilderImpl::add_numerical_feature(core::Numerical numerical) {
    auto result = m_caches.m_numerical_cache->insert(std::make_unique<NumericalFeature>(NumericalFeature(m_numerical_features.size(), std::move(numerical))));
    if (result.second) {
        m_numerical_features.push_back(result.first);
    }
    return result.first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_pos_condition(std::shared_ptr<const BooleanFeature> b) {
    return m_caches.m_condition_cache->insert(std::make_unique<PositiveBooleanCondition>(PositiveBooleanCondition(b))).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_neg_condition(std::shared_ptr<const BooleanFeature> b) {
    return m_caches.m_condition_cache->insert(std::make_unique<NegativeBooleanCondition>(NegativeBooleanCondition(b))).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_gt_condition(std::shared_ptr<const NumericalFeature> n) {
    return m_caches.m_condition_cache->insert(std::make_unique<GreaterNumericalCondition>(GreaterNumericalCondition(n))).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_eq_condition(std::shared_ptr<const NumericalFeature> n) {
    return m_caches.m_condition_cache->insert(std::make_unique<EqualNumericalCondition>(EqualNumericalCondition(n))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_pos_effect(std::shared_ptr<const BooleanFeature> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<PositiveBooleanEffect>(PositiveBooleanEffect(b))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_neg_effect(std::shared_ptr<const BooleanFeature> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<NegativeBooleanEffect>(NegativeBooleanEffect(b))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_bot_effect(std::shared_ptr<const BooleanFeature> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedBooleanEffect>(UnchangedBooleanEffect(b))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_inc_effect(std::shared_ptr<const NumericalFeature> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<IncrementNumericalEffect>(IncrementNumericalEffect(n))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_dec_effect(std::shared_ptr<const NumericalFeature> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<DecrementNumericalEffect>(DecrementNumericalEffect(n))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_bot_effect(std::shared_ptr<const NumericalFeature> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedNumericalEffect>(UnchangedNumericalEffect(n))).first;
}

std::shared_ptr<const Rule> PolicyBuilderImpl::add_rule(
    std::vector<std::shared_ptr<const BaseCondition>>&& conditions,
    std::vector<std::shared_ptr<const BaseEffect>>&& effects) {
    auto result = m_caches.m_rule_cache->insert(std::make_unique<Rule>(Rule(std::move(conditions), std::move(effects))));
    if (result.second) {
        m_rules.push_back(result.first);
    }
    return result.first;
}

Policy PolicyBuilderImpl::get_result() {
    return Policy(std::move(m_boolean_features), std::move(m_numerical_features), std::move(m_rules));
}

}
