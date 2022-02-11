#ifndef DLPLAN_SRC_POLICY_POLICY_IMPL_H_
#define DLPLAN_SRC_POLICY_POLICY_IMPL_H_

#include <unordered_set>
#include <vector>
#include <memory>

#include "evaluation_cache.h"


namespace dlplan {
namespace core {
    class State;
    class PerElementEvaluationCache;
    class InstanceInfo;
}
namespace policy {
class Rule;
class BooleanFeature;
class NumericalFeature;
class PolicyRoot;
class State;

class PolicyImpl {
private:
    const std::shared_ptr<const PolicyRoot> m_root;

    const std::shared_ptr<const core::InstanceInfo> m_instance_info;

    std::vector<std::shared_ptr<const BooleanFeature>> m_boolean_features;
    std::vector<std::shared_ptr<const NumericalFeature>> m_numerical_features;
    std::vector<std::shared_ptr<const Rule>> m_rules;

    // We have to store this in a shared_ptr
    // because Policy must be copieable
    // since we return it from the PolicyBuilder.
    std::shared_ptr<EvaluationCache> m_evaluation_cache;

public:
    PolicyImpl(
        std::shared_ptr<const PolicyRoot> root,
        std::shared_ptr<const core::InstanceInfo> instance_info,
        std::vector<std::shared_ptr<const BooleanFeature>>&& boolean_features,
        std::vector<std::shared_ptr<const NumericalFeature>>&& numerical_features,
        std::vector<std::shared_ptr<const Rule>>&& rules);

    std::shared_ptr<const Rule> evaluate_lazy(int source_index, const core::State& source, int target_index, const core::State& target);

    std::vector<std::shared_ptr<const Rule>> evaluate_conditions_eager(int source_index, const core::State& source);
    std::shared_ptr<const Rule> evaluate_effects_lazy(int source_index, const core::State& source, int target_index, const core::State& target, const std::vector<std::shared_ptr<const Rule>>& rules);

    std::string compute_repr() const;

    std::shared_ptr<const PolicyRoot> get_root() const;
    std::vector<std::shared_ptr<const BooleanFeature>> get_boolean_features() const;
    std::vector<std::shared_ptr<const NumericalFeature>> get_numerical_features() const;

    void clear_evaluation_cache();
};

}
}

#endif
