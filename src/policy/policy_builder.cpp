#include "policy_builder.h"

#include "condition.h"
#include "effect.h"

#include "../../include/dlplan/policy.h"

#include <stdexcept>


namespace dlplan::policy {

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_pos_condition(const std::shared_ptr<const core::Boolean>& boolean) {
    return m_caches.insert(std::make_unique<PositiveBooleanCondition>(boolean));
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_neg_condition(const std::shared_ptr<const core::Boolean>& boolean) {
    return m_caches.insert(std::make_unique<NegativeBooleanCondition>(boolean));
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_gt_condition(const std::shared_ptr<const core::Numerical>& numerical) {
    return m_caches.insert(std::make_unique<GreaterNumericalCondition>(numerical));
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_eq_condition(const std::shared_ptr<const core::Numerical>& numerical) {
    return m_caches.insert(std::make_unique<EqualNumericalCondition>(numerical));
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_pos_effect(const std::shared_ptr<const core::Boolean>& boolean) {
    return m_caches.insert(std::make_unique<PositiveBooleanEffect>(boolean));
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_neg_effect(const std::shared_ptr<const core::Boolean>& boolean) {
    return m_caches.insert(std::make_unique<NegativeBooleanEffect>(boolean));
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_bot_effect(const std::shared_ptr<const core::Boolean>& boolean) {
    return m_caches.insert(std::make_unique<UnchangedBooleanEffect>(boolean));
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_inc_effect(const std::shared_ptr<const core::Numerical>& numerical) {
    return m_caches.insert(std::make_unique<IncrementNumericalEffect>(numerical));
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_dec_effect(const std::shared_ptr<const core::Numerical>& numerical) {
    return m_caches.insert(std::make_unique<DecrementNumericalEffect>(numerical));
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_bot_effect(const std::shared_ptr<const core::Numerical>& numerical) {
    return m_caches.insert(std::make_unique<UnchangedNumericalEffect>(numerical));
}

std::shared_ptr<const Rule> PolicyBuilderImpl::add_rule(Conditions&& conditions, Effects&& effects) {
    //if (!(std::all_of(conditions.begin(), conditions.end(), [&](const auto& condition){ return m_conditions.count(condition); })
    //    && std::all_of(effects.begin(), effects.end(), [&](const auto& effect){ return m_effects.count(effect); }))) {
    //    throw std::runtime_error("PolicyBuilderImpl::add_rule - tried adding rules with condition or effect from different builder.");
    //}
    return m_caches.insert(std::unique_ptr<Rule>(new Rule(move(conditions), move(effects))));
}

std::shared_ptr<const Policy> PolicyBuilderImpl::add_policy(
    Rules&& rules) {
    //if (!std::all_of(rules.begin(), rules.end(), [&](const auto& rule){ return m_rules.count(rule); })) {
    //    throw std::runtime_error("PolicyBuilderImpl::add_policy - tried adding policy with rule from different builder.");
    //}
    return m_caches.insert(std::unique_ptr<Policy>(new Policy(std::move(rules))));
}

}
