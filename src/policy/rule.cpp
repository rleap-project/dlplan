#include "rule.h"

#include <sstream>

#include "algorithm"
#include "condition.h"
#include "effect.h"

#include "../include/dlplan/policy.h"


namespace dlplan::policy {

/**
 * For sorting conditions and effects according to their unique representation.
 */
template<typename T>
static std::vector<std::shared_ptr<const T>> sort(
    std::unordered_set<std::shared_ptr<const T>>&& set) {
    std::vector<std::shared_ptr<const T>> result(set.begin(), set.end());
    std::sort(
        result.begin(),
        result.end(),
        [](const auto& l, const auto& r){
            return l->compute_repr() < r->compute_repr();
        });
    return result;
}

RuleImpl::RuleImpl(
    std::shared_ptr<const PolicyRoot> root,
    std::unordered_set<std::shared_ptr<const BaseCondition>>&& conditions,
    std::unordered_set<std::shared_ptr<const BaseEffect>>&& effects)
    : m_root(root), m_conditions(sort(std::move(conditions))), m_effects(sort(std::move(effects))) { }

bool RuleImpl::evaluate_conditions(const core::State& source, EvaluationCaches& evaluation_caches) const {
    for (const auto& condition : m_conditions) {
        if (!condition->evaluate(source, evaluation_caches)) return false;
    }
    return true;
}

bool RuleImpl::evaluate_effects(const core::State& source, const core::State& target, EvaluationCaches& evaluation_caches) const {
    for (const auto& effect : m_effects) {
        if (!effect->evaluate(source, target, evaluation_caches)) return false;
    }
    return true;
}

std::string RuleImpl::compute_repr() const {
    std::stringstream ss;
    ss << "(:rule (:conditions ";
    for (const auto& c : m_conditions) {
        if (c != m_conditions.front()) {
            ss << " ";
        }
        ss << c->compute_repr();
    }
    ss << ") (:effects ";
    for (const auto& e : m_effects) {
        if (e != m_effects.front()) {
            ss << " ";
        }
        ss << e->compute_repr();
    }
    ss << "))";
    return ss.str();
}

std::shared_ptr<const PolicyRoot> RuleImpl::get_root() const {
    return m_root;
}

}