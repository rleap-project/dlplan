#include "../../include/dlplan/policy.h"

#include "condition.h"
#include "effect.h"

#include <sstream>


namespace dlplan::policy {

Rule::Rule(Conditions&& conditions, Effects&& effects)
    : m_conditions(std::move(conditions)), m_effects(std::move(effects)) {
}

Rule::~Rule() = default;

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

std::string Rule::compute_repr() const {
    std::stringstream ss;
    ss << "(:rule (:conditions ";
    // sort conditions by repr to obtain canonical representation
    std::vector<std::shared_ptr<const BaseCondition>> sorted_conditions(m_conditions.begin(), m_conditions.end());
    std::sort(sorted_conditions.begin(), sorted_conditions.end(), [&](const auto& l, const auto& r){
        return l->compute_repr() < r->compute_repr();
    });
    for (const auto& c : sorted_conditions) {
        ss << c->compute_repr();
        if (c != *sorted_conditions.rbegin()) {
            ss << " ";
        }
    }
    ss << ") (:effects ";
    // sort conditions by repr to obtain canonical representation
    std::vector<std::shared_ptr<const BaseEffect>> sorted_effects(m_effects.begin(), m_effects.end());
    std::sort(sorted_effects.begin(), sorted_effects.end(), [&](const auto& l, const auto& r){
        return l->compute_repr() < r->compute_repr();
    });
    for (const auto& e : sorted_effects) {
        ss << e->compute_repr();
        if (e != *sorted_effects.rbegin()) {
            ss << " ";
        }
    }
    ss << "))";
    return ss.str();
}

std::string Rule::str() const {
    std::stringstream ss;
    ss << "(:rule (:conditions ";
    for (const auto& c : m_conditions) {
        ss << c->str();
        if (c != *m_conditions.rbegin()) {
            ss << " ";
        }
    }
    ss << ") (:effects ";
    for (const auto& e : m_effects) {
        ss << e->str();
        if (e != *m_effects.rbegin()) {
            ss << " ";
        }
    }
    ss << "))";
    return ss.str();
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

void Rule::set_index(RuleIndex index) {
    m_index = index;
}

RuleIndex Rule::get_index() const {
    return m_index;
}

const Conditions& Rule::get_conditions() const {
    return m_conditions;
}

const Effects& Rule::get_effects() const {
    return m_effects;
}

}