#include "policy_builder.h"

#include "condition.h"
#include "effect.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {

template<typename T>
static std::vector<T> sort_values_by_increasing_complexity_then_repr(const std::vector<T>& values) {
    std::vector<T> result(values);
    std::sort(result.begin(), result.end(), [](T& l, T& r){
        if (l->get_base_feature()->compute_complexity() != r->get_base_feature()->compute_complexity()) {
            return l->get_base_feature()->compute_complexity() < r->get_base_feature()->compute_complexity();
        }
        return l->compute_repr() < r->compute_repr();
    });
    return result;
}


template<typename T>
static std::vector<T> sort_features_by_repr(const std::vector<T>& features) {
    std::vector<T> result(features);
    std::sort(result.begin(), result.end(), [](const auto& l, const auto& r){ return l->compute_repr() < r->compute_repr(); } );
    return result;
}


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
    return m_caches.m_condition_cache->insert(std::make_unique<PositiveBooleanCondition>(b)).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_neg_condition(std::shared_ptr<const core::Boolean> b) {
    return m_caches.m_condition_cache->insert(std::make_unique<NegativeBooleanCondition>(b)).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_gt_condition(std::shared_ptr<const core::Numerical> n) {
    return m_caches.m_condition_cache->insert(std::make_unique<GreaterNumericalCondition>(n)).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_eq_condition(std::shared_ptr<const core::Numerical> n) {
    return m_caches.m_condition_cache->insert(std::make_unique<EqualNumericalCondition>(n)).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_pos_effect(std::shared_ptr<const core::Boolean> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<PositiveBooleanEffect>(b)).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_neg_effect(std::shared_ptr<const core::Boolean> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<NegativeBooleanEffect>(b)).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_bot_effect(std::shared_ptr<const core::Boolean> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedBooleanEffect>(b)).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_inc_effect(std::shared_ptr<const core::Numerical> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<IncrementNumericalEffect>(n)).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_dec_effect(std::shared_ptr<const core::Numerical> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<DecrementNumericalEffect>(n)).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_bot_effect(std::shared_ptr<const core::Numerical> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedNumericalEffect>(n)).first;
}

std::shared_ptr<const Rule> PolicyBuilderImpl::add_rule(
    std::vector<std::shared_ptr<const BaseCondition>>&& conditions,
    std::vector<std::shared_ptr<const BaseEffect>>&& effects) {
    // Create rule in canonical representation.
    auto result = m_caches.m_rule_cache->insert(std::unique_ptr<Rule>(
        new Rule(sort_values_by_increasing_complexity_then_repr(conditions),
        sort_values_by_increasing_complexity_then_repr(effects))));
    if (result.second) {
        m_rules.push_back(result.first);
    }
    return result.first;
}

Policy PolicyBuilderImpl::get_result() {
    // Compute policy in canonical representation.
    PolicyBuilder builder;
    const auto sorted_boolean_features = sort_features_by_repr(m_boolean_features);
    std::vector<std::shared_ptr<const core::Boolean>> boolean_features;
    for (const auto& boolean : sorted_boolean_features) {
        boolean_features.push_back(builder.add_boolean_feature(*boolean));
    }
    const auto sorted_numerical_features = sort_features_by_repr(m_numerical_features);
    std::vector<std::shared_ptr<const core::Numerical>> numerical_features;
    for (const auto& numerical : sorted_numerical_features) {
        numerical_features.push_back(builder.add_numerical_feature(*numerical));
    }
    // TODO: use more sophisticated sorting, e.g., to ensure faster evaluation.
    std::vector<std::shared_ptr<const Rule>> sorted_rules(m_rules);
    std::sort(sorted_rules.begin(), sorted_rules.end(), [](const auto& l, const auto& r){ return l->compute_repr() < r->compute_repr(); } );
    std::vector<std::shared_ptr<const Rule>> rules;
    for (const auto& rule : sorted_rules) {
        rules.push_back(rule->copy_to_builder(builder));
    }
    return Policy(boolean_features, numerical_features, rules);
}

}
