#include "policy_builder.h"

#include "condition.h"
#include "effect.h"

//#include "../include/dlplan/core.h"
#include "../include/dlplan/policy.h"


namespace dlplan::policy {

std::shared_ptr<const BooleanFeature> PolicyBuilderImpl::add_boolean_feature(core::Boolean boolean) {
    m_boolean_features.push_back(std::make_shared<BooleanFeature>(BooleanFeature(m_root, m_boolean_features.size(), std::move(boolean))));
    return m_boolean_features.back();
}

std::shared_ptr<const NumericalFeature> PolicyBuilderImpl::add_numerical_feature(core::Numerical numerical) {
    m_numerical_features.push_back(std::make_shared<NumericalFeature>(NumericalFeature(m_root, m_numerical_features.size(), std::move(numerical))));
    return m_numerical_features.back();
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_b_pos_condition(std::shared_ptr<const BooleanFeature> b) {
    return m_caches.m_condition_cache->insert(std::make_unique<PositiveBooleanCondition>(PositiveBooleanCondition(m_root, b))).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_b_neg_condition(std::shared_ptr<const BooleanFeature> b) {
    return m_caches.m_condition_cache->insert(std::make_unique<NegativeBooleanCondition>(NegativeBooleanCondition(m_root, b))).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_n_gt_condition(std::shared_ptr<const NumericalFeature> n) {
    return m_caches.m_condition_cache->insert(std::make_unique<GreaterNumericalCondition>(GreaterNumericalCondition(m_root, n))).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_n_eq_condition(std::shared_ptr<const NumericalFeature> n) {
    return m_caches.m_condition_cache->insert(std::make_unique<EqualNumericalCondition>(EqualNumericalCondition(m_root, n))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_b_pos_effect(std::shared_ptr<const BooleanFeature> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<PositiveBooleanEffect>(PositiveBooleanEffect(m_root, b))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_b_neg_effect(std::shared_ptr<const BooleanFeature> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<NegativeBooleanEffect>(NegativeBooleanEffect(m_root, b))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_b_bot_effect(std::shared_ptr<const BooleanFeature> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedBooleanEffect>(UnchangedBooleanEffect(m_root, b))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_n_inc_effect(std::shared_ptr<const NumericalFeature> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<IncrementNumericalEffect>(IncrementNumericalEffect(m_root, n))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_n_dec_effect(std::shared_ptr<const NumericalFeature> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<DecrementNumericalEffect>(DecrementNumericalEffect(m_root, n))).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_n_bot_effect(std::shared_ptr<const NumericalFeature> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedNumericalEffect>(UnchangedNumericalEffect(m_root, n))).first;
}

std::shared_ptr<const Rule> PolicyBuilderImpl::add_rule(
    std::unordered_set<std::shared_ptr<const BaseCondition>>&& conditions,
    std::unordered_set<std::shared_ptr<const BaseEffect>>&& effects) {
    auto result = m_caches.m_rule_cache->insert(std::make_unique<Rule>(Rule(m_root, std::move(conditions), std::move(effects))));
    if (result.second) {
        m_rules.push_back(result.first);
    }
    return result.first;
}

Policy PolicyBuilderImpl::get_result() {
    return Policy(m_root, std::move(m_boolean_features), std::move(m_numerical_features), std::move(m_rules));
}

}
