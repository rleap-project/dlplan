#ifndef DLPLAN_SRC_POLICY_POLICY_BUILDER_H_
#define DLPLAN_SRC_POLICY_POLICY_BUILDER_H_

#include <memory>
#include <vector>
#include <set>

#include "cache.h"
#include "../../include/dlplan/core.h"
#include "../../include/dlplan/policy.h"


namespace dlplan::policy {
class PolicyBuilderImpl;
}


namespace boost::serialization {
    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::PolicyBuilderImpl& t, const unsigned int version);
}


namespace dlplan::policy {
class PolicyBuilderImpl {
private:
    Caches m_caches;

    template <typename Archive>
    friend void boost::serialization::serialize(Archive& ar, PolicyBuilderImpl& t, const unsigned int version);

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

#endif
