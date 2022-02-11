#include "policy_impl.h"

#include <algorithm>
#include <sstream>

#include "condition.h"
#include "effect.h"

#include "../include/dlplan/core.h"
#include "../include/dlplan/policy.h"


namespace dlplan::policy {

PolicyImpl::PolicyImpl(
    std::shared_ptr<const PolicyRoot> root,
    std::vector<std::shared_ptr<const BooleanFeature>>&& boolean_features,
    std::vector<std::shared_ptr<const NumericalFeature>>&& numerical_features,
    std::vector<std::shared_ptr<const Rule>>&& rules)
    : m_root(root),
      m_boolean_features(std::move(boolean_features)),
      m_numerical_features(std::move(numerical_features)),
      m_rules(std::move(rules)),
      m_evaluation_cache(m_boolean_features.size(), m_numerical_features.size()) { }

std::shared_ptr<const Rule> PolicyImpl::evaluate_lazy(int source_index, const core::State& source, int target_index, const core::State& target, core::PerElementEvaluationCache& element_cache) {
    if (source_index < 0 || target_index < 0) {
        throw std::runtime_error("PolicyImpl::evaluate_lazy: source or target index cannot be negative.");
    }
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_index, source, m_evaluation_cache, element_cache) && r->evaluate_effects(source_index, source, target_index, target, m_evaluation_cache, element_cache)) {
            return r;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<const Rule>> PolicyImpl::evaluate_conditions_eager(int source_index, const core::State& source, core::PerElementEvaluationCache& element_cache) {
    if (source_index < 0) {
        throw std::runtime_error("PolicyImpl::evaluate_conditions_eager: source index cannot be negative.");
    }
    std::vector<std::shared_ptr<const Rule>> result;
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_index, source, m_evaluation_cache, element_cache)) {
            result.push_back(r);
        }
    }
    return result;
}

std::shared_ptr<const Rule> PolicyImpl::evaluate_effects_lazy(int source_index, const core::State& source, int target_index, const core::State& target, core::PerElementEvaluationCache& element_cache, const std::vector<std::shared_ptr<const Rule>>& rules) {
    if (source_index < 0 || target_index < 0) {
        throw std::runtime_error("PolicyImpl::evaluate_effects_lazy: source or target index cannot be negative.");
    }
    //std::cout << source_index << " " << target_index << std::endl;
    for (const auto& r : rules) {
        if (r->evaluate_effects(source_index, source, target_index, target, m_evaluation_cache, element_cache)) {
            return r;
        }
    }
    //std::cout << std::endl;
    return nullptr;
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

std::vector<std::shared_ptr<const BooleanFeature>> PolicyImpl::get_boolean_features() const {
    return m_boolean_features;
}

std::vector<std::shared_ptr<const NumericalFeature>> PolicyImpl::get_numerical_features() const {
    return m_numerical_features;
}

void PolicyImpl::clear_evaluation_cache() {
    m_evaluation_cache = EvaluationCache(m_boolean_features.size(), m_numerical_features.size());
}

}