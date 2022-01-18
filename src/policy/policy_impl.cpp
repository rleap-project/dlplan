#include "policy_impl.h"

#include <sstream>

#include "condition.h"
#include "effect.h"

#include "../include/dlplan/core.h"


namespace dlplan::policy {

PolicyImpl::PolicyImpl() : m_root(std::make_shared<PolicyRoot>(PolicyRoot())) { }

std::shared_ptr<Feature<bool>> PolicyImpl::add_boolean_feature() {
    m_boolean_features.push_back(std::make_shared<Feature<bool>>(Feature<bool>(m_root, m_boolean_features.size())));
    return m_boolean_features.back();

}

std::shared_ptr<Feature<int>> PolicyImpl::add_numerical_feature() {
    m_numerical_features.push_back(std::make_shared<Feature<int>>(Feature<int>(m_root, m_numerical_features.size())));
    return m_numerical_features.back();
}

std::shared_ptr<const BaseCondition> PolicyImpl::add_b_pos_condition(std::shared_ptr<const Feature<bool>> b) {
    return m_caches.m_condition_cache->insert(std::make_unique<PositiveBooleanCondition>(PositiveBooleanCondition(b))).first;
}

std::shared_ptr<const BaseCondition> PolicyImpl::add_b_neg_condition(std::shared_ptr<const Feature<bool>> b) {
    return m_caches.m_condition_cache->insert(std::make_unique<NegativeBooleanCondition>(NegativeBooleanCondition(b))).first;
}

std::shared_ptr<const BaseCondition> PolicyImpl::add_n_gt_condition(std::shared_ptr<const Feature<int>> n) {
    return m_caches.m_condition_cache->insert(std::make_unique<GreaterNumericalCondition>(GreaterNumericalCondition(n))).first;
}

std::shared_ptr<const BaseCondition> PolicyImpl::add_n_eq_condition(std::shared_ptr<const Feature<int>> n) {
    return m_caches.m_condition_cache->insert(std::make_unique<EqualNumericalCondition>(EqualNumericalCondition(n))).first;
}

std::shared_ptr<const BaseEffect> PolicyImpl::add_b_pos_effect(std::shared_ptr<const Feature<bool>> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<PositiveBooleanEffect>(PositiveBooleanEffect(b))).first;
}

std::shared_ptr<const BaseEffect> PolicyImpl::add_b_neg_effect(std::shared_ptr<const Feature<bool>> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<NegativeBooleanEffect>(NegativeBooleanEffect(b))).first;
}

std::shared_ptr<const BaseEffect> PolicyImpl::add_b_bot_effect(std::shared_ptr<const Feature<bool>> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedBooleanEffect>(UnchangedBooleanEffect(b))).first;
}

std::shared_ptr<const BaseEffect> PolicyImpl::add_n_inc_effect(std::shared_ptr<const Feature<int>> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<IncrementNumericalEffect>(IncrementNumericalEffect(n))).first;
}

std::shared_ptr<const BaseEffect> PolicyImpl::add_n_dec_effect(std::shared_ptr<const Feature<int>> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<DecrementNumericalEffect>(DecrementNumericalEffect(n))).first;
}

std::shared_ptr<const BaseEffect> PolicyImpl::add_n_bot_effect(std::shared_ptr<const Feature<int>> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedNumericalEffect>(UnchangedNumericalEffect(n))).first;
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

void PolicyImpl::reset_cached_source_evaluations() {
    for (auto& b : m_boolean_features) b->reset_cached_source_evaluation();
    for (auto& n : m_numerical_features) n->reset_cached_source_evaluation();
}

void PolicyImpl::reset_cached_target_evaluations() {
    for (auto& b : m_boolean_features) b->reset_cached_target_evaluation();
    for (auto& n : m_numerical_features) n->reset_cached_target_evaluation();
}

bool PolicyImpl::evaluate_lazy(const core::State& source, const core::State& target) {
    for (auto& r : m_rules) {
        if (r->is_condition_satisfied(source) && r->is_effect_satisfied(source, target)) return true;
    }
    return false;
}

std::string PolicyImpl::str() const {
    std::stringstream ss;
    ss << "(:general_policy\n";
    // boolean features
    ss << "(:boolean_features " << m_boolean_features.size() << ")\n";
    // numerical_features
    ss << "(:numerical_features " << m_numerical_features.size() << ")\n";
    // rules
    for (const auto& r : m_rules) {
        ss << r->str() << "\n";
    }
    ss << ")";
    return ss.str();
}

std::shared_ptr<const PolicyRoot> PolicyImpl::get_root() const {
    return m_root;
}

std::vector<std::shared_ptr<Feature<bool>>> PolicyImpl::get_boolean_features() const {
    return m_boolean_features;
}

std::vector<std::shared_ptr<Feature<int>>> PolicyImpl::get_numerical_features() const {
    return m_numerical_features;
}

}