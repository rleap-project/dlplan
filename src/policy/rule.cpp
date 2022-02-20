#include "../../include/dlplan/policy.h"

#include <sstream>

#include "algorithm"
#include "condition.h"
#include "effect.h"


namespace dlplan::policy {

/**
 * For sorting conditions and effects according to their unique representation.
 */
template<typename pT>
static std::vector<pT> sort(const std::vector<pT>& set) {
    std::vector<pT> result(set.begin(), set.end());
    std::sort(
        result.begin(),
        result.end(),
        [](const auto& l, const auto& r){
            return l->compute_repr() < r->compute_repr();
        });
    return result;
}

Rule::Rule(
    std::shared_ptr<const PolicyRoot> root,
    std::vector<std::shared_ptr<const BaseCondition>>&& conditions,
    std::vector<std::shared_ptr<const BaseEffect>>&& effects)
    : m_root(root), m_conditions(std::move(conditions)), m_effects(std::move(effects)) { }

Rule::Rule(Rule&& other) = default;

Rule& Rule::operator=(Rule&& other) = default;

Rule::~Rule() = default;

bool Rule::evaluate_conditions(evaluator::EvaluationContext& source_context) const {
    for (const auto& condition : m_conditions) {
        if (!condition->evaluate(source_context)) return false;
    }
    return true;
}

bool Rule::evaluate_effects(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const {
    for (const auto& effect : m_effects) {
        if (!effect->evaluate(source_context, target_context)) return false;
    }
    return true;
}

std::string Rule::compute_repr() const {
    std::stringstream ss;
    ss << "(:rule (:conditions ";
    const auto sorted_conditions = sort(m_conditions);
    for (const auto& c : sorted_conditions) {
        ss << c->compute_repr();
        if (c != sorted_conditions.back()) {
            ss << " ";
        }
    }
    ss << ") (:effects ";
    auto sorted_effects = sort(m_effects);
    for (const auto& e : sorted_effects) {
        ss << e->compute_repr();
        if (e != sorted_effects.back()) {
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
        if (c != m_conditions.back()) {
            ss << " ";
        }
    }
    ss << ") (:effects ";
    for (const auto& e : m_effects) {
        ss << e->str();
        if (e != m_effects.back()) {
            ss << " ";
        }
    }
    ss << "))";
    return ss.str();
}

std::shared_ptr<const PolicyRoot> Rule::get_root() const {
    return m_root;
}

}