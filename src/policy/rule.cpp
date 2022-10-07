#include "../../include/dlplan/policy.h"

#include "condition.h"
#include "effect.h"

#include <sstream>


namespace dlplan::policy {

/**
 * For sorting conditions and effects according to their unique representation.
 */
template<typename pT>
static std::vector<pT> sort_by_feature_index_then_repr(const std::vector<pT>& set) {
    std::vector<pT> result(set.begin(), set.end());
    std::sort(
        result.begin(),
        result.end(),
        [](const auto& l, const auto& r){
            if (l->get_base_feature()->get_index() != r->get_base_feature()->get_index()) {
                return l->get_base_feature()->get_index() < r->get_base_feature()->get_index();
            }
            return l->compute_repr() < r->compute_repr();
        });
    return result;
}

Rule::Rule(
    std::vector<std::shared_ptr<const BaseCondition>>&& conditions,
    std::vector<std::shared_ptr<const BaseEffect>>&& effects,
    int index)
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
        if (c != m_conditions.back()) {
            ss << " ";
        }
    }
    ss << ") (:effects ";
    for (const auto& e : m_effects) {
        ss << e->compute_repr();
        if (e != m_effects.back()) {
            ss << " ";
        }
    }
    ss << "))";
    return ss.str();
}

std::string Rule::str() const {
    std::stringstream ss;
    ss << "(:rule (:conditions ";
    // canonical representation of conditions
    const auto sorted_conditions = sort_by_feature_index_then_repr(m_conditions);
    for (const auto& c : sorted_conditions) {
        ss << c->compute_repr();
        if (c != sorted_conditions.back()) {
            ss << " ";
        }
    }
    ss << ") (:effects ";
    // canonical representation of effects
    const auto sorted_effects = sort_by_feature_index_then_repr(m_effects);
    for (const auto& e : sorted_effects) {
        ss << e->compute_repr();
        if (e != sorted_effects.back()) {
            ss << " ";
        }
    }
    ss << "))";
    return ss.str();
}

std::shared_ptr<const Rule> Rule::copy_to_builder(PolicyBuilder& policy_builder) const {
    std::vector<std::shared_ptr<const BaseCondition>> conditions;
    conditions.reserve(m_conditions.size());
    for (const auto& condition : m_conditions) {
        conditions.push_back(condition->copy_to_builder(policy_builder));
    }
    std::vector<std::shared_ptr<const BaseEffect>> effects;
    effects.reserve(m_effects.size());
    for (const auto& effect : m_effects) {
        effects.push_back(effect->copy_to_builder(policy_builder));
    }
    return policy_builder.add_rule(std::move(conditions), std::move(effects));
}

void Rule::set_index(int index) {
    m_index = index;
}

int Rule::get_index() const {
    return m_index;
}

std::vector<std::shared_ptr<const BaseCondition>> Rule::get_conditions() const {
    return m_conditions;
}

std::vector<std::shared_ptr<const BaseEffect>> Rule::get_effects() const {
    return m_effects;
}

}