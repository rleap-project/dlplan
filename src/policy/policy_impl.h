#ifndef DLPLAN_SRC_POLICY_POLICY_IMPL_H_
#define DLPLAN_SRC_POLICY_POLICY_IMPL_H_

#include <unordered_set>
#include <vector>
#include <memory>

#include "cache.h"


namespace dlplan::policy {
class Rule;
class BooleanFeature;
class NumericalFeature;
class PolicyRoot;


class PolicyImpl {
private:
    const std::shared_ptr<const PolicyRoot> m_root;

    Caches m_caches;

    std::vector<std::shared_ptr<BooleanFeature>> m_boolean_features;
    std::vector<std::shared_ptr<NumericalFeature>> m_numerical_features;
    std::vector<std::shared_ptr<Rule>> m_rules;

public:
    PolicyImpl();

    std::shared_ptr<BooleanFeature> add_boolean_feature(core::Boolean boolean);
    std::shared_ptr<NumericalFeature> add_numerical_feature(core::Numerical numerical);

    std::shared_ptr<const BaseCondition> add_b_pos_condition(std::shared_ptr<const BooleanFeature> b);
    std::shared_ptr<const BaseCondition> add_b_neg_condition(std::shared_ptr<const BooleanFeature> b);
    std::shared_ptr<const BaseCondition> add_n_gt_condition(std::shared_ptr<const NumericalFeature> n);
    std::shared_ptr<const BaseCondition> add_n_eq_condition(std::shared_ptr<const NumericalFeature> n);
    std::shared_ptr<const BaseEffect> add_b_pos_effect(std::shared_ptr<const BooleanFeature> b);
    std::shared_ptr<const BaseEffect> add_b_neg_effect(std::shared_ptr<const BooleanFeature> b);
    std::shared_ptr<const BaseEffect> add_b_bot_effect(std::shared_ptr<const BooleanFeature> b);
    std::shared_ptr<const BaseEffect> add_n_inc_effect(std::shared_ptr<const NumericalFeature> n);
    std::shared_ptr<const BaseEffect> add_n_dec_effect(std::shared_ptr<const NumericalFeature> n);
    std::shared_ptr<const BaseEffect> add_n_bot_effect(std::shared_ptr<const NumericalFeature> n);

    std::shared_ptr<const Rule> add_rule(
        std::unordered_set<std::shared_ptr<const BaseCondition>>&& conditions,
        std::unordered_set<std::shared_ptr<const BaseEffect>>&& effects);

    bool evaluate_lazy(const State& source, const State& target);

    std::string str() const;

    std::shared_ptr<const PolicyRoot> get_root() const;
    std::vector<std::shared_ptr<BooleanFeature>> get_boolean_features() const;
    std::vector<std::shared_ptr<NumericalFeature>> get_numerical_features() const;
};

}

#endif
