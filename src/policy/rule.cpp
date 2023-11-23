#include "../../include/dlplan/policy.h"

#include "../../include/dlplan/utils/hash.h"

#include <algorithm>
#include <sstream>


namespace dlplan::policy {
Rule::Rule(int identifier, const Conditions& conditions, const Effects& effects)
    : Base<Rule>(identifier), m_conditions(conditions), m_effects(effects) {
}

Rule::Rule(const Rule& other) = default;

Rule& Rule::operator=(const Rule& other) = default;

Rule::Rule(Rule&& other) = default;

Rule& Rule::operator=(Rule&& other) = default;

Rule::~Rule() = default;

bool Rule::are_equal_impl(const Rule& other) const {
    if (this != &other) {
        return m_conditions == other.m_conditions
            && m_effects == other.m_effects;
    }
    return true;
}

size_t Rule::hash_impl() const {
    return hash_combine(
        hash_set(m_conditions),
        hash_set(m_effects));
}

bool Rule::evaluate_conditions(const core::State& source_state) const {
    for (const auto& condition : m_conditions) {
        if (!condition->evaluate(source_state)) return false;
    }
    return true;
}

bool Rule::evaluate_conditions(const core::State& source_state, core::DenotationsCaches& caches) const {
    for (const auto& condition : m_conditions) {
        if (!condition->evaluate(source_state, caches)) return false;
    }
    return true;
}

bool Rule::evaluate_effects(const core::State& source_state, const core::State& target_state) const {
    for (const auto& effect : m_effects) {
        if (!effect->evaluate(source_state, target_state)) return false;
    }
    return true;
}

bool Rule::evaluate_effects(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    for (const auto& effect : m_effects) {
        if (!effect->evaluate(source_state, target_state, caches)) return false;
    }
    return true;
}


void Rule::str_impl(std::stringstream& out) const {
    out << "(:rule (:conditions ";
    for (const auto& c : m_conditions) {
        out << c->str();
        if (c != *m_conditions.rbegin()) {
            out << " ";
        }
    }
    out << ") (:effects ";
    for (const auto& e : m_effects) {
        out << e->str();
        if (e != *m_effects.rbegin()) {
            out << " ";
        }
    }
    out << "))";
}

int Rule::compute_evaluate_time_score() const {
    int score = 0;
    for (const auto& condition : m_conditions) {
        score += condition->compute_evaluate_time_score();
    }
    for (const auto& effect : m_effects) {
        score += effect->compute_evaluate_time_score();
    }
    return score;
}

const Conditions& Rule::get_conditions() const {
    return m_conditions;
}

const Effects& Rule::get_effects() const {
    return m_effects;
}

}
