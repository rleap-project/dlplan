#ifndef DLPLAN_SRC_POLICY_POLICY_BUILDER_H_
#define DLPLAN_SRC_POLICY_POLICY_BUILDER_H_

#include "../../include/dlplan/utils/factory.h"
#include "../../include/dlplan/core.h"
#include "../../include/dlplan/policy.h"
#include "../../include/dlplan/policy/condition.h"
#include "../../include/dlplan/policy/effect.h"


#include <memory>
#include <vector>
#include <set>


namespace dlplan::policy {
class PolicyFactoryImpl {
private:
    std::shared_ptr<core::SyntacticElementFactory> m_element_factory;

    ReferenceCountedObjectFactory<NamedBoolean
        , NamedNumerical
        , NamedConcept
        , NamedRole
        , PositiveBooleanCondition
        , NegativeBooleanCondition
        , GreaterNumericalCondition
        , EqualNumericalCondition
        , GreaterConceptCondition
        , EqualConceptCondition
        , PositiveBooleanEffect
        , NegativeBooleanEffect
        , UnchangedBooleanEffect
        , IncrementNumericalEffect
        , IncrementOrUnchangedNumericalEffect
        , DecrementNumericalEffect
        , DecrementOrUnchangedNumericalEffect
        , UnchangedNumericalEffect
        , GreaterNumericalEffect
        , EqualNumericalEffect
        , IncrementConceptEffect
        , DecrementConceptEffect
        , UnchangedConceptEffect
        , GreaterConceptEffect
        , EqualConceptEffect
        , Rule
        , Policy> m_cache;

public:
    explicit PolicyFactoryImpl(std::shared_ptr<core::SyntacticElementFactory> element_factory);

    std::shared_ptr<const Policy> parse_policy(
        PolicyFactory& parent,
        const std::string& description,
        const std::string& filename);

    std::shared_ptr<const Policy> parse_policy(
        PolicyFactory& parent,
        iterator_type& iter, iterator_type end,
        const std::string& filename);

    std::shared_ptr<const NamedBoolean> make_boolean(const std::string& key, const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const NamedNumerical> make_numerical(const std::string& key, const std::shared_ptr<const core::Numerical>& numerical);
    std::shared_ptr<const NamedConcept> make_concept(const std::string& key, const std::shared_ptr<const core::Concept>& concept_);
    std::shared_ptr<const NamedRole> make_role(const std::string& key, const std::shared_ptr<const core::Role>& role);

    std::shared_ptr<const BaseCondition> make_pos_condition(const std::shared_ptr<const NamedBoolean>& boolean);
    std::shared_ptr<const BaseCondition> make_neg_condition(const std::shared_ptr<const NamedBoolean>& boolean);
    std::shared_ptr<const BaseCondition> make_gt_condition(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseCondition> make_eq_condition(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseCondition> make_gt_condition(const std::shared_ptr<const NamedConcept>& concept_);
    std::shared_ptr<const BaseCondition> make_eq_condition(const std::shared_ptr<const NamedConcept>& concept_);
    std::shared_ptr<const BaseEffect> make_pos_effect(const std::shared_ptr<const NamedBoolean>& boolean);
    std::shared_ptr<const BaseEffect> make_neg_effect(const std::shared_ptr<const NamedBoolean>& boolean);
    std::shared_ptr<const BaseEffect> make_bot_effect(const std::shared_ptr<const NamedBoolean>& boolean);
    std::shared_ptr<const BaseEffect> make_inc_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_inc_bot_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_dec_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_dec_bot_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_bot_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_gt_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_eq_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_inc_effect(const std::shared_ptr<const NamedConcept>& concept_);
    std::shared_ptr<const BaseEffect> make_dec_effect(const std::shared_ptr<const NamedConcept>& concept_);
    std::shared_ptr<const BaseEffect> make_bot_effect(const std::shared_ptr<const NamedConcept>& concept_);
    std::shared_ptr<const BaseEffect> make_gt_effect(const std::shared_ptr<const NamedConcept>& concept_);
    std::shared_ptr<const BaseEffect> make_eq_effect(const std::shared_ptr<const NamedConcept>& concept_);

    std::shared_ptr<const Rule> make_rule(const Conditions& conditions, const Effects& effects);

    std::shared_ptr<const Policy> make_policy(const Rules& rules);

    std::shared_ptr<core::SyntacticElementFactory> get_element_factory() const;
};

}

#endif
