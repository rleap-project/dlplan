#include "../../include/dlplan/policy.h"

#include "condition.h"
#include "effect.h"

#include <sstream>


namespace dlplan::policy {

Rule::Rule(Conditions&& conditions, Effects&& effects, int index)
    : m_conditions(std::move(conditions)), m_effects(std::move(effects)), m_index(index) { }

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
    for (const auto& c : m_conditions) {
        ss << c->compute_repr();
        if (c != *m_conditions.rbegin()) {
            ss << " ";
        }
    }
    ss << ") (:effects ";
    for (const auto& e : m_effects) {
        ss << e->compute_repr();
        if (e != *m_effects.rbegin()) {
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
        ss << " ";
    }
    ss << ") (:effects ";
    for (const auto& e : m_effects) {
        ss << e->str();
        ss << " ";
    }
    ss << "))";
    return ss.str();
}

std::shared_ptr<const Rule> Rule::copy_to_builder(PolicyBuilder& policy_builder) const {
    Conditions conditions;
    for (const auto& condition : m_conditions) {
        conditions.insert(condition->copy_to_builder(policy_builder));
    }
    Effects effects;
    for (const auto& effect : m_effects) {
        effects.insert(effect->copy_to_builder(policy_builder));
    }
    return policy_builder.add_rule(std::move(conditions), std::move(effects));
}

void Rule::set_index(int index) {
    m_index = index;
}

int Rule::get_index() const {
    return m_index;
}

Conditions Rule::get_conditions() const {
    return m_conditions;
}

Effects Rule::get_effects() const {
    return m_effects;
}

}