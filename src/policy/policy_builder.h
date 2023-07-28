#ifndef DLPLAN_SRC_POLICY_POLICY_BUILDER_H_
#define DLPLAN_SRC_POLICY_POLICY_BUILDER_H_

#include "../utils/cache.h"

#include "../../include/dlplan/policy.h"

#include <memory>
#include <vector>
#include <set>

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
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, BaseCondition>> m_condition_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, BaseEffect>> m_effect_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Rule>> m_rule_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Policy>> m_policy_cache;

public:
    std::shared_ptr<const BaseCondition> add_pos_condition(const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const BaseCondition> add_neg_condition(const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const BaseCondition> add_gt_condition(const std::shared_ptr<const core::Numerical>& numerical);
    std::shared_ptr<const BaseCondition> add_eq_condition(const std::shared_ptr<const core::Numerical>& numerical);
    std::shared_ptr<const BaseEffect> add_pos_effect(const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const BaseEffect> add_neg_effect(const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const BaseEffect> add_bot_effect(const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const BaseEffect> add_inc_effect(const std::shared_ptr<const core::Numerical>& numerical);
    std::shared_ptr<const BaseEffect> add_dec_effect(const std::shared_ptr<const core::Numerical>& numerical);
    std::shared_ptr<const BaseEffect> add_bot_effect(const std::shared_ptr<const core::Numerical>& numerical);

    std::shared_ptr<const Rule> add_rule(Conditions&& conditions, Effects&& effects);

    std::shared_ptr<const Policy> add_policy(Rules&& rules);
};

}
}

#endif
