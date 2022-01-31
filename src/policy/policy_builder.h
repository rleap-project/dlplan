#ifndef DLPLAN_SRC_POLICY_POLICY_BUILDER_H_
#define DLPLAN_SRC_POLICY_POLICY_BUILDER_H_

#include <memory>
#include <unordered_set>
#include <vector>

#include "cache.h"


namespace dlplan {
namespace core {
    class Boolean;
    class Numerical;
}
namespace policy {
class PolicyRoot;
class BooleanFeature;
class NumericalFeature;
class BaseCondition;
class BaseEffect;
class Rule;
class Policy;

class PolicyBuilderImpl {
private:
    const std::shared_ptr<const PolicyRoot> m_root;

    Caches m_caches;

    std::vector<std::shared_ptr<const BooleanFeature>> m_boolean_features;
    std::vector<std::shared_ptr<const NumericalFeature>> m_numerical_features;

    std::vector<std::shared_ptr<const Rule>> m_rules;

public:
    std::shared_ptr<const BooleanFeature> add_boolean_feature(core::Boolean boolean);
    std::shared_ptr<const NumericalFeature> add_numerical_feature(core::Numerical numerical);

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

    Policy get_result();
};

}
}

#endif
