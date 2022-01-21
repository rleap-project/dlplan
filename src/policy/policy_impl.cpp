#include "policy_impl.h"

#include <sstream>

#include "condition.h"
#include "effect.h"

#include "../include/dlplan/core.h"
#include "../include/dlplan/policy.h"


namespace dlplan::policy {

PolicyImpl::PolicyImpl()
    : m_root(std::make_shared<PolicyRoot>(PolicyRoot())) { }

std::shared_ptr<BooleanFeature> PolicyImpl::add_boolean_feature(core::Boolean boolean) {
    m_boolean_features.push_back(std::make_shared<BooleanFeature>(BooleanFeature(m_root, m_boolean_features.size(), std::move(boolean))));
    return m_boolean_features.back();
}

std::shared_ptr<NumericalFeature> PolicyImpl::add_numerical_feature(core::Numerical numerical) {
    m_numerical_features.push_back(std::make_shared<NumericalFeature>(NumericalFeature(m_root, m_numerical_features.size(), std::move(numerical))));
    return m_numerical_features.back();
}

std::shared_ptr<const BaseCondition> PolicyImpl::add_b_pos_condition(std::shared_ptr<const BooleanFeature> b) {
    return m_caches.m_condition_cache->insert(std::make_unique<PositiveBooleanCondition>(PositiveBooleanCondition(m_root, b))).first;
}

std::shared_ptr<const BaseCondition> PolicyImpl::add_b_neg_condition(std::shared_ptr<const BooleanFeature> b) {
    return m_caches.m_condition_cache->insert(std::make_unique<NegativeBooleanCondition>(NegativeBooleanCondition(m_root, b))).first;
}

std::shared_ptr<const BaseCondition> PolicyImpl::add_n_gt_condition(std::shared_ptr<const NumericalFeature> n) {
    return m_caches.m_condition_cache->insert(std::make_unique<GreaterNumericalCondition>(GreaterNumericalCondition(m_root, n))).first;
}

std::shared_ptr<const BaseCondition> PolicyImpl::add_n_eq_condition(std::shared_ptr<const NumericalFeature> n) {
    return m_caches.m_condition_cache->insert(std::make_unique<EqualNumericalCondition>(EqualNumericalCondition(m_root, n))).first;
}

std::shared_ptr<const BaseEffect> PolicyImpl::add_b_pos_effect(std::shared_ptr<const BooleanFeature> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<PositiveBooleanEffect>(PositiveBooleanEffect(m_root, b))).first;
}

std::shared_ptr<const BaseEffect> PolicyImpl::add_b_neg_effect(std::shared_ptr<const BooleanFeature> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<NegativeBooleanEffect>(NegativeBooleanEffect(m_root, b))).first;
}

std::shared_ptr<const BaseEffect> PolicyImpl::add_b_bot_effect(std::shared_ptr<const BooleanFeature> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedBooleanEffect>(UnchangedBooleanEffect(m_root, b))).first;
}

std::shared_ptr<const BaseEffect> PolicyImpl::add_n_inc_effect(std::shared_ptr<const NumericalFeature> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<IncrementNumericalEffect>(IncrementNumericalEffect(m_root, n))).first;
}

std::shared_ptr<const BaseEffect> PolicyImpl::add_n_dec_effect(std::shared_ptr<const NumericalFeature> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<DecrementNumericalEffect>(DecrementNumericalEffect(m_root, n))).first;
}

std::shared_ptr<const BaseEffect> PolicyImpl::add_n_bot_effect(std::shared_ptr<const NumericalFeature> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedNumericalEffect>(UnchangedNumericalEffect(m_root, n))).first;
}

std::shared_ptr<const Rule> PolicyImpl::add_rule(
    std::unordered_set<std::shared_ptr<const BaseCondition>>&& conditions,
    std::unordered_set<std::shared_ptr<const BaseEffect>>&& effects) {
    auto result = m_caches.m_rule_cache->insert(std::make_unique<Rule>(Rule(m_root, std::move(conditions), std::move(effects))));
    if (result.second) {
        m_rules.push_back(result.first);
    }
    return result.first;
}

bool PolicyImpl::evaluate(const State& source, const State& target) {
    for (auto& r : m_rules) {
        if (r->evaluate_conditions(source) && r->evaluate_effects(source, target)) return true;
    }
    return false;
}

std::string PolicyImpl::compute_repr() const {
    std::stringstream ss;
    ss << "(:policy\n";
    // boolean features
    ss << "(:boolean_features ";
    for (const auto& b : m_boolean_features) {
        ss << "\"" << b->compute_repr() << "\"";
        if (b != m_boolean_features.back()) ss << " ";
    }
    ss << ")\n";
    // numerical_features
    ss << "(:numerical_features ";
    for (const auto& n : m_numerical_features) {
        ss << "\"" << n->compute_repr() << "\"";
        if (n != m_numerical_features.back()) ss << " ";
    }
    ss << ")\n";
    // rules
    for (const auto& r : m_rules) {
        ss << r->compute_repr() << "\n";
    }
    ss << ")";
    return ss.str();
}

std::shared_ptr<const PolicyRoot> PolicyImpl::get_root() const {
    return m_root;
}

std::vector<std::shared_ptr<BooleanFeature>> PolicyImpl::get_boolean_features() const {
    return m_boolean_features;
}

std::vector<std::shared_ptr<NumericalFeature>> PolicyImpl::get_numerical_features() const {
    return m_numerical_features;
}

}