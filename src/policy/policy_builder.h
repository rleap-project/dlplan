#ifndef DLPLAN_SRC_POLICY_POLICY_BUILDER_H_
#define DLPLAN_SRC_POLICY_POLICY_BUILDER_H_

#include "cache.h"

#include <memory>
#include <vector>


namespace dlplan {
namespace core {
    class Boolean;
    class Numerical;
}
namespace policy {
class PolicyRoot;
class BaseCondition;
class BaseEffect;
class Rule;
class Policy;
class PolicyMinimizer;

class PolicyBuilderImpl {
private:
    Caches m_caches;

    core::Booleans m_boolean_features;
    core::Numericals m_numerical_features;

    Rules m_rules;

    friend class PolicyMinimizer;

public:
    std::shared_ptr<const core::Boolean> add_boolean_feature(core::Boolean boolean);
    std::shared_ptr<const core::Numerical> add_numerical_feature(core::Numerical numerical);

    std::shared_ptr<const BaseCondition> add_pos_condition(std::shared_ptr<const core::Boolean> b);
    std::shared_ptr<const BaseCondition> add_neg_condition(std::shared_ptr<const core::Boolean> b);
    std::shared_ptr<const BaseCondition> add_gt_condition(std::shared_ptr<const core::Numerical> n);
    std::shared_ptr<const BaseCondition> add_eq_condition(std::shared_ptr<const core::Numerical> n);
    std::shared_ptr<const BaseEffect> add_pos_effect(std::shared_ptr<const core::Boolean> b);
    std::shared_ptr<const BaseEffect> add_neg_effect(std::shared_ptr<const core::Boolean> b);
    std::shared_ptr<const BaseEffect> add_bot_effect(std::shared_ptr<const core::Boolean> b);
    std::shared_ptr<const BaseEffect> add_inc_effect(std::shared_ptr<const core::Numerical> n);
    std::shared_ptr<const BaseEffect> add_dec_effect(std::shared_ptr<const core::Numerical> n);
    std::shared_ptr<const BaseEffect> add_bot_effect(std::shared_ptr<const core::Numerical> n);

    std::shared_ptr<const Rule> add_rule(
        Conditions&& conditions,
        Effects&& effects);

    Policy get_result();
};

}
}

#endif
