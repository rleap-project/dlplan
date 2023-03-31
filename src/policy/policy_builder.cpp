#include "policy_builder.h"

#include "condition.h"
#include "effect.h"

#include "../../include/dlplan/policy.h"

#include <stdexcept>


namespace dlplan::policy {

std::shared_ptr<const core::Boolean> PolicyBuilderImpl::add_boolean_feature(core::Boolean boolean) {
    return *m_booleans.insert(m_caches.m_boolean_cache->insert(std::make_unique<core::Boolean>(boolean)).first).first;
}

std::shared_ptr<const core::Numerical> PolicyBuilderImpl::add_numerical_feature(core::Numerical numerical) {
    return *m_numericals.insert(m_caches.m_numerical_cache->insert(std::make_unique<core::Numerical>(numerical)).first).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_pos_condition(std::shared_ptr<const core::Boolean> b) {
    if (!m_booleans.count(b)) {
        throw std::runtime_error("PolicyBuilderImpl::add_pos_condition - tried adding condition with Boolean from different builder.");
    }
    return *m_conditions.insert(m_caches.m_condition_cache->insert(std::make_unique<PositiveBooleanCondition>(b)).first).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_neg_condition(std::shared_ptr<const core::Boolean> b) {
    if (!m_booleans.count(b)) {
        throw std::runtime_error("PolicyBuilderImpl::add_neg_condition - tried adding condition with Boolean from different builder.");
    }
    return *m_conditions.insert(m_caches.m_condition_cache->insert(std::make_unique<NegativeBooleanCondition>(b)).first).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_gt_condition(std::shared_ptr<const core::Numerical> n) {
    if (!m_numericals.count(n)) {
        throw std::runtime_error("PolicyBuilderImpl::add_gt_condition - tried adding condition with numerical from different builder.");
    }
    return *m_conditions.insert(m_caches.m_condition_cache->insert(std::make_unique<GreaterNumericalCondition>(n)).first).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_eq_condition(std::shared_ptr<const core::Numerical> n) {
    if (!m_numericals.count(n)) {
        throw std::runtime_error("PolicyBuilderImpl::add_eq_condition - tried adding condition with numerical from different builder.");
    }
    return *m_conditions.insert(m_caches.m_condition_cache->insert(std::make_unique<EqualNumericalCondition>(n)).first).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_pos_effect(std::shared_ptr<const core::Boolean> b) {
    if (!m_booleans.count(b)) {
        throw std::runtime_error("PolicyBuilderImpl::add_pos_effect - tried adding effect with Boolean from different builder.");
    }
    return *m_effects.insert(m_caches.m_effect_cache->insert(std::make_unique<PositiveBooleanEffect>(b)).first).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_neg_effect(std::shared_ptr<const core::Boolean> b) {
    if (!m_booleans.count(b)) {
        throw std::runtime_error("PolicyBuilderImpl::add_neg_effect - tried adding effect with Boolean from different builder.");
    }
    return *m_effects.insert(m_caches.m_effect_cache->insert(std::make_unique<NegativeBooleanEffect>(b)).first).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_bot_effect(std::shared_ptr<const core::Boolean> b) {
    if (!m_booleans.count(b)) {
        throw std::runtime_error("PolicyBuilderImpl::add_bot_effect - tried adding effect with Boolean from different builder.");
    }
    return *m_effects.insert(m_caches.m_effect_cache->insert(std::make_unique<UnchangedBooleanEffect>(b)).first).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_inc_effect(std::shared_ptr<const core::Numerical> n) {
    if (!m_numericals.count(n)) {
        throw std::runtime_error("PolicyBuilderImpl::add_inc_effect - tried adding effect with numerical from different builder.");
    }
    return *m_effects.insert(m_caches.m_effect_cache->insert(std::make_unique<IncrementNumericalEffect>(n)).first).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_dec_effect(std::shared_ptr<const core::Numerical> n) {
    if (!m_numericals.count(n)) {
        throw std::runtime_error("PolicyBuilderImpl::add_dec_effect - tried adding effect with numerical from different builder.");
    }
    return *m_effects.insert(m_caches.m_effect_cache->insert(std::make_unique<DecrementNumericalEffect>(n)).first).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_bot_effect(std::shared_ptr<const core::Numerical> n) {
    if (!m_numericals.count(n)) {
        throw std::runtime_error("PolicyBuilderImpl::add_bot_effect - tried adding effect with numerical from different builder.");
    }
    return *m_effects.insert(m_caches.m_effect_cache->insert(std::make_unique<UnchangedNumericalEffect>(n)).first).first;
}

std::shared_ptr<const Rule> PolicyBuilderImpl::add_rule(Conditions&& conditions, Effects&& effects) {
    if (!(std::all_of(conditions.begin(), conditions.end(), [&](const auto& condition){ return m_conditions.count(condition); })
        && std::all_of(effects.begin(), effects.end(), [&](const auto& effect){ return m_effects.count(effect); }))) {
        throw std::runtime_error("PolicyBuilderImpl::add_rule - tried adding rules with condition or effect from different builder.");
    }
    return *m_rules.insert(m_caches.m_rule_cache->insert(std::unique_ptr<Rule>(new Rule(move(conditions), move(effects)))).first).first;
}

Policy PolicyBuilderImpl::get_result() {
    return Policy(Rules(m_rules));
}


Booleans PolicyBuilderImpl::get_booleans() const {
    return m_booleans;
}

Numericals PolicyBuilderImpl::get_numericals() const {
    return m_numericals;
}

}
