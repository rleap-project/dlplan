#include "../../include/dlplan/policy.h"

#include "condition.h"
#include "effect.h"

#include "../../include/dlplan/core.h"

#include <sstream>


namespace dlplan::policy {

Policy::Policy() = default;

Policy::Policy(Rules&& rules)
    : m_rules(move(rules)) { }

Policy::Policy(const Policy& other) = default;

Policy& Policy::operator=(const Policy& other) = default;

Policy::Policy(Policy&& other) = default;

Policy& Policy::operator=(Policy&& other) = default;

Policy::~Policy() = default;

std::shared_ptr<const Rule> Policy::evaluate_lazy(const core::State& source_state, const core::State& target_state) const {
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_state) && r->evaluate_effects(source_state, target_state)) {
            return r;
        }
    }
    return nullptr;
}

std::shared_ptr<const Rule> Policy::evaluate_lazy(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_state, caches) && r->evaluate_effects(source_state, target_state, caches)) {
            return r;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<const Rule>> Policy::evaluate_conditions_eager(const core::State& source_state) const {
    std::vector<std::shared_ptr<const Rule>> result;
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_state)) {
            result.push_back(r);
        }
    }
    return result;
}

std::vector<std::shared_ptr<const Rule>> Policy::evaluate_conditions_eager(const core::State& source_state, core::DenotationsCaches& caches) const {
    std::vector<std::shared_ptr<const Rule>> result;
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_state, caches)) {
            result.push_back(r);
        }
    }
    return result;
}

std::shared_ptr<const Rule> Policy::evaluate_effects_lazy(const core::State& source_state, const core::State& target_state, const std::vector<std::shared_ptr<const Rule>>& rules) const {
    for (const auto& r : rules) {
        if (r->evaluate_effects(source_state, target_state)) {
            return r;
        }
    }
    return nullptr;
}

std::shared_ptr<const Rule> Policy::evaluate_effects_lazy(const core::State& source_state, const core::State& target_state, const std::vector<std::shared_ptr<const Rule>>& rules, core::DenotationsCaches& caches) const {
    for (const auto& r : rules) {
        if (r->evaluate_effects(source_state, target_state, caches)) {
            return r;
        }
    }
    return nullptr;
}


std::string Policy::compute_repr() const {
    // Canonical representation
    std::stringstream ss;
    ss << "(:policy\n";
    std::vector<std::shared_ptr<const Rule>> sorted_rules(m_rules.begin(), m_rules.end());
    std::sort(sorted_rules.begin(), sorted_rules.end(), [](const auto& r1, const auto& r2){ return r1->compute_repr() < r2->compute_repr(); });
    for (const auto& r : sorted_rules) {
        ss << r->compute_repr() << "\n";
    }
    ss << ")";
    return ss.str();
}

std::string Policy::str() const {
    // Canonical representation
    std::stringstream ss;
    // Parse everything into a builder to be able to print boolean, numerical,
    // and rules from builder with correct indexing since parsing policy into builder
    // changes indices of features in features and rules.
    PolicyBuilder builder;
    ss << "(:policy\n";
    copy_to_builder(builder);
    const auto booleans = builder.get_booleans();
    std::vector<std::shared_ptr<const core::Boolean>> sorted_booleans(booleans.begin(), booleans.end());
    std::sort(sorted_booleans.begin(), sorted_booleans.end(), [](const auto& b1, const auto& b2){ return b1->get_index() < b2->get_index(); });
    ss << "(:boolean_features ";
    for (const auto& boolean : sorted_booleans) {
        ss << "\"" << boolean->compute_repr() << "\"";
        if (boolean != *booleans.rbegin()) ss << " ";
    }
    ss << ")\n";
    const auto numericals = builder.get_numericals();
    std::vector<std::shared_ptr<const core::Numerical>> sorted_numericals(numericals.begin(), numericals.end());
    std::sort(sorted_numericals.begin(), sorted_numericals.end(), [](const auto& n1, const auto& n2){ return n1->get_index() < n2->get_index(); });
    ss << "(:numerical_features ";
    for (const auto& numerical : sorted_numericals) {
        ss << "\"" << numerical->compute_repr() << "\"";
        if (numerical != *numericals.rbegin()) ss << " ";
    }
    ss << ")\n";
    const auto rules = builder.get_result().get_rules();
    std::vector<std::shared_ptr<const Rule>> sorted_rules(rules.begin(), rules.end());
    std::sort(sorted_rules.begin(), sorted_rules.end(), [](const auto& r1, const auto& r2){ return r1->compute_repr() < r2->compute_repr(); });
    for (const auto& r : sorted_rules) {
        ss << r->str() << "\n";
    }
    ss << ")";
    return ss.str();
}

Policy Policy::copy_to_builder(PolicyBuilder& policy_builder) const {
    Rules rules;
    for (const auto& rule : m_rules) {
        rules.insert(rule->copy_to_builder(policy_builder));
    }
    return policy_builder.get_result();
}

void Policy::set_index(int index) {
    m_index = index;
}

int Policy::get_index() const {
    return m_index;
}

const Rules& Policy::get_rules() const {
    return m_rules;
}

}