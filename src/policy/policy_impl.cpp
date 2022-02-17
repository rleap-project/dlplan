#include "../include/dlplan/policy.h"

#include <algorithm>
#include <sstream>

#include "condition.h"
#include "effect.h"

#include "../include/dlplan/core.h"


namespace dlplan::policy {

Policy::Policy(
    std::shared_ptr<const PolicyRoot> root,
    std::vector<std::shared_ptr<const BooleanFeature>>&& boolean_features,
    std::vector<std::shared_ptr<const NumericalFeature>>&& numerical_features,
    std::vector<std::shared_ptr<const Rule>>&& rules)
    : m_root(root),
      m_boolean_features(std::move(boolean_features)),
      m_numerical_features(std::move(numerical_features)),
      m_rules(std::move(rules)),
      m_cache(m_boolean_features.size(), m_numerical_features.size()) { }

Policy::Policy(const Policy& other) = default;

Policy& Policy::operator=(const Policy& other) = default;

Policy::Policy(Policy&& other) = default;

Policy& Policy::operator=(Policy&& other) = default;

Policy::~Policy() = default;


std::shared_ptr<const Rule> Policy::evaluate_lazy(int source_index, const core::State& source, int target_index, const core::State& target) {
    if (source_index < 0 || target_index < 0) {
        throw std::runtime_error("Policy::evaluate_lazy: source or target index cannot be negative.");
    }
    evaluator::EvaluationContext source_context(source_index, source, m_cache);
    evaluator::EvaluationContext target_context(target_index, target, m_cache);
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_context) && r->evaluate_effects(source_context, target_context)) {
            return r;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<const Rule>> Policy::evaluate_conditions_eager(int source_index, const core::State& source) {
    if (source_index < 0) {
        throw std::runtime_error("Policy::evaluate_conditions_eager: source index cannot be negative.");
    }
    evaluator::EvaluationContext source_context(source_index, source, m_cache);
    std::vector<std::shared_ptr<const Rule>> result;
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_context)) {
            result.push_back(r);
        }
    }
    return result;
}

std::shared_ptr<const Rule> Policy::evaluate_effects_lazy(int source_index, const core::State& source, int target_index, const core::State& target, const std::vector<std::shared_ptr<const Rule>>& rules) {
    if (source_index < 0 || target_index < 0) {
        throw std::runtime_error("Policy::evaluate_effects_lazy: source or target index cannot be negative.");
    }
    // assert(std::all_of(rules.begin(), rules.end(), [&, source_index](const auto& r){ r->evaluate_conditions(source_index, source); }));
    evaluator::EvaluationContext source_context(source_index, source, m_cache);
    evaluator::EvaluationContext target_context(target_index, target, m_cache);
    for (const auto& r : rules) {
        if (r->evaluate_effects(source_context, target_context)) {
            return r;
        }
    }
    return nullptr;
}


std::string Policy::compute_repr() const {
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

std::shared_ptr<const PolicyRoot> Policy::get_root() const {
    return m_root;
}

std::vector<std::shared_ptr<const BooleanFeature>> Policy::get_boolean_features() const {
    return m_boolean_features;
}

std::vector<std::shared_ptr<const NumericalFeature>> Policy::get_numerical_features() const {
    return m_numerical_features;
}

}