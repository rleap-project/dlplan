#ifndef DLPLAN_SRC_POLICY_POLICY_BUILDER_H_
#define DLPLAN_SRC_POLICY_POLICY_BUILDER_H_

#include <memory>
#include <vector>
#include <set>

#include "include/dlplan/core.h"
#include "include/dlplan/policy.h"

#include "cache.h"


namespace dlplan::policy {
class PolicyFactoryImpl;
}


namespace boost::serialization {
    class access;

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::PolicyFactoryImpl& t, const unsigned int version);
}


namespace dlplan::policy {
class PolicyFactoryImpl {
private:
    std::shared_ptr<core::SyntacticElementFactory> m_element_factory;
    Caches m_caches;

    /// @brief Constructor for serialization.
    PolicyFactoryImpl();

    friend class boost::serialization::access;
    template <typename Archive>
    friend void boost::serialization::serialize(Archive& ar, PolicyFactoryImpl& t, const unsigned int version);

public:
    explicit PolicyFactoryImpl(std::shared_ptr<core::SyntacticElementFactory> element_factory);

    std::shared_ptr<const Policy> parse_policy(
        PolicyFactory& parent,
        const std::string& description,
        const std::string& filename);

    std::shared_ptr<const Policy> parse_policy(
        PolicyFactory& parent,
        common::parsers::iterator_type& iter, common::parsers::iterator_type end,
        const std::string& filename);

    std::shared_ptr<const BaseCondition> make_pos_condition(const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const BaseCondition> make_neg_condition(const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const BaseCondition> make_gt_condition(const std::shared_ptr<const core::Numerical>& numerical);
    std::shared_ptr<const BaseCondition> make_eq_condition(const std::shared_ptr<const core::Numerical>& numerical);
    std::shared_ptr<const BaseEffect> make_pos_effect(const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const BaseEffect> make_neg_effect(const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const BaseEffect> make_bot_effect(const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const BaseEffect> make_inc_effect(const std::shared_ptr<const core::Numerical>& numerical);
    std::shared_ptr<const BaseEffect> make_dec_effect(const std::shared_ptr<const core::Numerical>& numerical);
    std::shared_ptr<const BaseEffect> make_bot_effect(const std::shared_ptr<const core::Numerical>& numerical);

    std::shared_ptr<const Rule> make_rule(const Conditions& conditions, const Effects& effects);

    std::shared_ptr<const Policy> make_policy(const Rules& rules);

    std::shared_ptr<core::SyntacticElementFactory> get_element_factory() const;
};

}

#endif
