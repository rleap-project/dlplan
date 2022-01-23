#ifndef DLPLAN_SRC_POLICY_POLICY_IMPL_H_
#define DLPLAN_SRC_POLICY_POLICY_IMPL_H_

#include <unordered_set>
#include <vector>
#include <memory>


namespace dlplan::policy {
class Rule;
class BooleanFeature;
class NumericalFeature;
class PolicyRoot;
class EvaluationCaches;
class State;

// TODO: we must move the construction in a builder because the number of features must be known in advance
// to initialize the sizes of the caches.
class PolicyImpl {
private:
    const std::shared_ptr<const PolicyRoot> m_root;

    std::vector<std::shared_ptr<const BooleanFeature>> m_boolean_features;
    std::vector<std::shared_ptr<const NumericalFeature>> m_numerical_features;
    std::vector<std::shared_ptr<const Rule>> m_rules;

    std::shared_ptr<EvaluationCaches> m_evaluation_caches;

public:
    PolicyImpl(
        std::shared_ptr<const PolicyRoot> root,
        std::vector<std::shared_ptr<const BooleanFeature>>&& boolean_features,
        std::vector<std::shared_ptr<const NumericalFeature>>&& numerical_features,
        std::vector<std::shared_ptr<const Rule>>&& rules);

    bool evaluate(const State& source, const State& target);

    std::string compute_repr() const;

    std::shared_ptr<const PolicyRoot> get_root() const;
    std::vector<std::shared_ptr<const BooleanFeature>> get_boolean_features() const;
    std::vector<std::shared_ptr<const NumericalFeature>> get_numerical_features() const;
};

}

#endif
