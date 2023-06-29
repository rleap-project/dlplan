#include "../../include/dlplan/policy.h"

#include "condition.h"
#include "effect.h"

#include "../../include/dlplan/core.h"

#include <sstream>


namespace dlplan::policy {

Policy::Policy() = default;

Policy::Policy(Rules&& rules)
    : m_rules(move(rules)) {
    // Retrieve boolean and numericals from the rules.
    for (const auto& rule : m_rules) {
        for (const auto& condition : rule->get_conditions()) {
            const auto boolean = condition->get_boolean();
            if (boolean) {
                m_booleans.insert(boolean);
            }
            const auto numerical = condition->get_numerical();
            if (numerical) {
                m_numericals.insert(numerical);
            }
        }
        for (const auto& effect : rule->get_effects()) {
            const auto boolean = effect->get_boolean();
            if (boolean) {
                m_booleans.insert(boolean);
            }
            const auto numerical = effect->get_numerical();
            if (numerical) {
                m_numericals.insert(numerical);
            }
        }
    }
}

Policy::Policy(const Policy& other) = default;

Policy& Policy::operator=(const Policy& other) = default;

Policy::Policy(Policy&& other) = default;

Policy& Policy::operator=(Policy&& other) = default;

Policy::~Policy() = default;

std::shared_ptr<const Rule> Policy::evaluate(const core::State& source_state, const core::State& target_state) const {
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_state) && r->evaluate_effects(source_state, target_state)) {
            return r;
        }
    }
    return nullptr;
}

std::shared_ptr<const Rule> Policy::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_state, caches) && r->evaluate_effects(source_state, target_state, caches)) {
            return r;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<const Rule>> Policy::evaluate_conditions(const core::State& source_state) const {
    std::vector<std::shared_ptr<const Rule>> result;
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_state)) {
            result.push_back(r);
        }
    }
    return result;
}

std::vector<std::shared_ptr<const Rule>> Policy::evaluate_conditions(const core::State& source_state, core::DenotationsCaches& caches) const {
    std::vector<std::shared_ptr<const Rule>> result;
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_state, caches)) {
            result.push_back(r);
        }
    }
    return result;
}

std::shared_ptr<const Rule> Policy::evaluate_effects(const core::State& source_state, const core::State& target_state, const std::vector<std::shared_ptr<const Rule>>& rules) const {
    for (const auto& r : rules) {
        if (r->evaluate_effects(source_state, target_state)) {
            return r;
        }
    }
    return nullptr;
}

std::shared_ptr<const Rule> Policy::evaluate_effects(const core::State& source_state, const core::State& target_state, const std::vector<std::shared_ptr<const Rule>>& rules, core::DenotationsCaches& caches) const {
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
    std::stringstream ss;
    ss << "(:policy\n";
    ss << "(:booleans ";
    for (const auto& boolean : m_booleans) {
        ss << "(" << boolean->get_index() << " \"" << boolean->compute_repr() << "\")";
        if (boolean != *m_booleans.rbegin()) ss << " ";
    }
    ss << ")\n";
    ss << "(:numericals ";
    for (const auto& numerical : m_numericals) {
        ss << "(" << numerical->get_index() << " \"" << numerical->compute_repr() << "\")";
        if (numerical != *m_numericals.rbegin()) ss << " ";
    }
    ss << ")\n";
    for (const auto& rule : m_rules) {
        ss << rule->str() << "\n";
    }
    ss << ")";
    return ss.str();
}

int Policy::compute_evaluate_time_score() const {
    int score = 0;
    for (const auto& rule : m_rules) {
        score += rule->compute_evaluate_time_score();
    }
    return score;
}

void Policy::set_index(PolicyIndex index) {
    m_index = index;
}

PolicyIndex Policy::get_index() const {
    return m_index;
}

const Booleans& Policy::get_booleans() const {
    return m_booleans;
}

const Numericals& Policy::get_numericals() const {
    return m_numericals;
}

const Rules& Policy::get_rules() const {
    return m_rules;
}

}