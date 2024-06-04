#include "../../include/dlplan/policy.h"

#include "policy_factory.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <iostream>


using namespace dlplan;


namespace dlplan::policy {


BaseCondition::BaseCondition(int identifier) : Base<BaseCondition>(identifier) { }

BaseCondition::~BaseCondition() = default;


BaseEffect::BaseEffect(int identifier) : Base<BaseEffect>(identifier) { }

BaseEffect::~BaseEffect() = default;


PolicyFactory::PolicyFactory(std::shared_ptr<core::SyntacticElementFactory> element_factory)
    : m_pImpl(PolicyFactoryImpl(element_factory)) { }

PolicyFactory::PolicyFactory(const PolicyFactory& other)
    : m_pImpl(*other.m_pImpl) { }

PolicyFactory& PolicyFactory::operator=(const PolicyFactory& other) {
    if (this != &other) {
        *m_pImpl = *other.m_pImpl;
    }
    return *this;
}

PolicyFactory::PolicyFactory(PolicyFactory&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

PolicyFactory& PolicyFactory::operator=(PolicyFactory&& other) {
    if (this != &other) {
        std::swap(*m_pImpl, *other.m_pImpl);
    }
    return *this;
}

PolicyFactory::~PolicyFactory() = default;

std::shared_ptr<const Policy> PolicyFactory::parse_policy(
    const std::string& description,
    const std::string& filename) {
    return m_pImpl->parse_policy(*this, description, filename);
}

std::shared_ptr<const Policy> PolicyFactory::parse_policy(
    iterator_type& iter, iterator_type end,
    const std::string& filename) {
    return m_pImpl->parse_policy(*this, iter, end, filename);
}

std::shared_ptr<const NamedBoolean> PolicyFactory::make_boolean(const std::string& key, const std::shared_ptr<const core::Boolean>& boolean) {
    return m_pImpl->make_boolean(key, boolean);
}

std::shared_ptr<const NamedNumerical> PolicyFactory::make_numerical(const std::string& key, const std::shared_ptr<const core::Numerical>& numerical) {
    return m_pImpl->make_numerical(key, numerical);
}

std::shared_ptr<const NamedConcept> PolicyFactory::make_concept(const std::string& key, const std::shared_ptr<const core::Concept>& concept_) {
    return m_pImpl->make_concept(key, concept_);
}

std::shared_ptr<const NamedRole> PolicyFactory::make_role(const std::string& key, const std::shared_ptr<const core::Role>& role) {
    return m_pImpl->make_role(key, role);
}

std::shared_ptr<const BaseCondition> PolicyFactory::make_pos_condition(const std::shared_ptr<const NamedBoolean>& boolean) {
    return m_pImpl->make_pos_condition(boolean);
}

std::shared_ptr<const BaseCondition> PolicyFactory::make_neg_condition(const std::shared_ptr<const NamedBoolean>& boolean) {
    return m_pImpl->make_neg_condition(boolean);
}

std::shared_ptr<const BaseCondition> PolicyFactory::make_gt_condition(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_pImpl->make_gt_condition(numerical);
}

std::shared_ptr<const BaseCondition> PolicyFactory::make_eq_condition(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_pImpl->make_eq_condition(numerical);
}

std::shared_ptr<const BaseCondition> PolicyFactory::make_gt_condition(const std::shared_ptr<const NamedConcept>& concept_) {
    return m_pImpl->make_gt_condition(concept_);
}

std::shared_ptr<const BaseCondition> PolicyFactory::make_eq_condition(const std::shared_ptr<const NamedConcept>& concept_) {
    return m_pImpl->make_eq_condition(concept_);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_pos_effect(const std::shared_ptr<const NamedBoolean>& boolean) {
    return m_pImpl->make_pos_effect(boolean);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_neg_effect(const std::shared_ptr<const NamedBoolean>& boolean) {
    return m_pImpl->make_neg_effect(boolean);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_bot_effect(const std::shared_ptr<const NamedBoolean>& boolean) {
    return m_pImpl->make_bot_effect(boolean);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_inc_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_pImpl->make_inc_effect(numerical);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_inc_bot_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_pImpl->make_inc_bot_effect(numerical);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_dec_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_pImpl->make_dec_effect(numerical);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_dec_bot_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_pImpl->make_dec_bot_effect(numerical);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_bot_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_pImpl->make_bot_effect(numerical);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_gt_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_pImpl->make_gt_effect(numerical);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_eq_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_pImpl->make_eq_effect(numerical);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_inc_effect(const std::shared_ptr<const NamedConcept>& concept_) {
    return m_pImpl->make_inc_effect(concept_);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_dec_effect(const std::shared_ptr<const NamedConcept>& concept_) {
    return m_pImpl->make_dec_effect(concept_);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_bot_effect(const std::shared_ptr<const NamedConcept>& concept_) {
    return m_pImpl->make_bot_effect(concept_);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_gt_effect(const std::shared_ptr<const NamedConcept>& concept_) {
    return m_pImpl->make_gt_effect(concept_);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_eq_effect(const std::shared_ptr<const NamedConcept>& concept_) {
    return m_pImpl->make_eq_effect(concept_);
}

std::shared_ptr<const Rule> PolicyFactory::make_rule(
    const Conditions& conditions,
    const Effects& effects) {
    return m_pImpl->make_rule(conditions, effects);
}

std::shared_ptr<const Policy> PolicyFactory::make_policy(
    const Rules& rules) {
    return m_pImpl->make_policy(rules);
}

std::shared_ptr<core::SyntacticElementFactory> PolicyFactory::get_element_factory() const {
    return m_pImpl->get_element_factory();
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::policy::NamedBoolean>>::operator()(
        const std::shared_ptr<const dlplan::policy::NamedBoolean>& left_boolean,
        const std::shared_ptr<const dlplan::policy::NamedBoolean>& right_boolean) const {
        return *left_boolean < *right_boolean;
    }

    bool less<std::shared_ptr<const dlplan::policy::NamedNumerical>>::operator()(
        const std::shared_ptr<const dlplan::policy::NamedNumerical>& left_numerical,
        const std::shared_ptr<const dlplan::policy::NamedNumerical>& right_numerical) const {
        return *left_numerical < *right_numerical;
    }

    bool less<std::shared_ptr<const dlplan::policy::NamedConcept>>::operator()(
        const std::shared_ptr<const dlplan::policy::NamedConcept>& left_concept,
        const std::shared_ptr<const dlplan::policy::NamedConcept>& right_concept) const {
        return *left_concept < *right_concept;
    }

    bool less<std::shared_ptr<const dlplan::policy::NamedRole>>::operator()(
        const std::shared_ptr<const dlplan::policy::NamedRole>& left_role,
        const std::shared_ptr<const dlplan::policy::NamedRole>& right_role) const {
        return *left_role < *right_role;
    }

    bool less<std::shared_ptr<const dlplan::policy::Rule>>::operator()(
        const std::shared_ptr<const dlplan::policy::Rule>& left_rule,
        const std::shared_ptr<const dlplan::policy::Rule>& right_rule) const {
        return *left_rule < *right_rule;
    }

    bool less<std::shared_ptr<const dlplan::policy::Policy>>::operator()(
        const std::shared_ptr<const dlplan::policy::Policy>& left_policy,
        const std::shared_ptr<const dlplan::policy::Policy>& right_policy) const {
        return *left_policy < *right_policy;
    }


    std::size_t hash<dlplan::policy::NamedBoolean>::operator()(const dlplan::policy::NamedBoolean& boolean) const {
        return boolean.hash();
    }

    std::size_t hash<dlplan::policy::NamedNumerical>::operator()(const dlplan::policy::NamedNumerical& numerical) const {
        return numerical.hash();
    }

    std::size_t hash<dlplan::policy::NamedConcept>::operator()(const dlplan::policy::NamedConcept& concept_) const {
        return concept_.hash();
    }

    std::size_t hash<dlplan::policy::NamedRole>::operator()(const dlplan::policy::NamedRole& role) const {
        return role.hash();
    }

    std::size_t hash<dlplan::policy::Rule>::operator()(const dlplan::policy::Rule& rule) const {
        return rule.hash();
    }

    std::size_t hash<dlplan::policy::Policy>::operator()(const dlplan::policy::Policy& policy) const {
        return policy.hash();
    }
}
