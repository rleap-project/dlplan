#include "../../include/dlplan/policy.h"

#include <algorithm>
#include <sstream>

#include "condition.h"
#include "effect.h"

#include "../../include/dlplan/core.h"


namespace dlplan::policy {

Policy::Policy(
    const std::vector<std::shared_ptr<const core::Boolean>>& boolean_features,
    const std::vector<std::shared_ptr<const core::Numerical>>& numerical_features,
    const std::vector<std::shared_ptr<const Rule>>& rules)
    : m_boolean_features(boolean_features),
      m_numerical_features(numerical_features),
      m_rules(rules) { }

Policy::Policy(const Policy& other) = default;

Policy& Policy::operator=(const Policy& other) = default;

Policy::Policy(Policy&& other) = default;

Policy& Policy::operator=(Policy&& other) = default;

Policy::~Policy() = default;


std::shared_ptr<const Rule> Policy::evaluate_lazy(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) {
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_context) && r->evaluate_effects(source_context, target_context)) {
            return r;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<const Rule>> Policy::evaluate_conditions_eager(evaluator::EvaluationContext& source_context) {
    std::vector<std::shared_ptr<const Rule>> result;
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_context)) {
            result.push_back(r);
        }
    }
    return result;
}

std::shared_ptr<const Rule> Policy::evaluate_effects_lazy(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context, const std::vector<std::shared_ptr<const Rule>>& rules) {
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

std::string Policy::str() const {
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
        ss << r->str() << "\n";
    }
    ss << ")";
    return ss.str();
}

std::vector<std::shared_ptr<const Rule>> Policy::get_rules() const {
    return m_rules;
}

std::vector<std::shared_ptr<const core::Boolean>> Policy::get_boolean_features() const {
    return m_boolean_features;
}

std::vector<std::shared_ptr<const core::Numerical>> Policy::get_numerical_features() const {
    return m_numerical_features;
}

}