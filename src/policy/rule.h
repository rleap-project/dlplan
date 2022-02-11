#ifndef DLPLAN_SRC_POLICY_RULE_H_
#define DLPLAN_SRC_POLICY_RULE_H_

#include <unordered_set>
#include <vector>
#include <memory>


namespace dlplan {
namespace core {
    class State;
    class PerElementEvaluationCache;
}
namespace policy {
class PolicyRoot;
class BaseCondition;
class BaseEffect;
class EvaluationCache;


class RuleImpl {
private:
    std::shared_ptr<const PolicyRoot> m_root;

    std::vector<std::shared_ptr<const BaseCondition>> m_conditions;
    std::vector<std::shared_ptr<const BaseEffect>> m_effects;

public:
    // Do we want to pass conditions and effects as ordered sets?
    // How does this work with python bindings then?
    RuleImpl(
        std::shared_ptr<const PolicyRoot> root,
        std::unordered_set<std::shared_ptr<const BaseCondition>>&& conditions,
        std::unordered_set<std::shared_ptr<const BaseEffect>>&& effects);

    bool evaluate_conditions(int source_index, const core::State& source, EvaluationCache& evaluation_cache, core::PerElementEvaluationCache& element_cache) const;
    bool evaluate_effects(int source_index, const core::State& source, int target_index, const core::State& target, EvaluationCache& evaluation_cache, core::PerElementEvaluationCache& element_cache) const;

    std::string compute_repr() const;

    std::shared_ptr<const PolicyRoot> get_root() const;
};

}
}

#endif
