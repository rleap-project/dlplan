#include "policy_factory.h"

#include <stdexcept>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "include/dlplan/policy.h"

#include "condition.h"
#include "effect.h"


using namespace dlplan::common::parsers;


namespace dlplan::policy {

PolicyFactoryImpl::PolicyFactoryImpl() : element_factory(nullptr) { }

PolicyFactoryImpl::PolicyFactoryImpl(std::shared_ptr<core::SyntacticElementFactory> element_factory)
    : element_factory(element_factory) { }

std::shared_ptr<const Policy> PolicyFactoryImpl::parse_policy(
    const std::string& description,
    const std::string& filename) {

}

std::shared_ptr<const Policy> PolicyFactoryImpl::parse_policy(
    iterator_type& iter, iterator_type end,
    const std::string& filename) {

}

std::shared_ptr<const BaseCondition> PolicyFactoryImpl::make_pos_condition(const std::shared_ptr<const core::Boolean>& boolean) {
    return m_caches.m_condition_cache->insert(std::make_unique<PositiveBooleanCondition>(boolean, m_caches.m_condition_cache->size())).first;
}

std::shared_ptr<const BaseCondition> PolicyFactoryImpl::make_neg_condition(const std::shared_ptr<const core::Boolean>& boolean) {
    return m_caches.m_condition_cache->insert(std::make_unique<NegativeBooleanCondition>(boolean, m_caches.m_condition_cache->size())).first;
}

std::shared_ptr<const BaseCondition> PolicyFactoryImpl::make_gt_condition(const std::shared_ptr<const core::Numerical>& numerical) {
    return m_caches.m_condition_cache->insert(std::make_unique<GreaterNumericalCondition>(numerical, m_caches.m_condition_cache->size())).first;
}

std::shared_ptr<const BaseCondition> PolicyFactoryImpl::make_eq_condition(const std::shared_ptr<const core::Numerical>& numerical) {
    return m_caches.m_condition_cache->insert(std::make_unique<EqualNumericalCondition>(numerical, m_caches.m_condition_cache->size())).first;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_pos_effect(const std::shared_ptr<const core::Boolean>& boolean) {
    return m_caches.m_effect_cache->insert(std::make_unique<PositiveBooleanEffect>(boolean, m_caches.m_effect_cache->size())).first;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_neg_effect(const std::shared_ptr<const core::Boolean>& boolean) {
    return m_caches.m_effect_cache->insert(std::make_unique<NegativeBooleanEffect>(boolean, m_caches.m_effect_cache->size())).first;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_bot_effect(const std::shared_ptr<const core::Boolean>& boolean) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedBooleanEffect>(boolean, m_caches.m_effect_cache->size())).first;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_inc_effect(const std::shared_ptr<const core::Numerical>& numerical) {
    return m_caches.m_effect_cache->insert(std::make_unique<IncrementNumericalEffect>(numerical, m_caches.m_effect_cache->size())).first;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_dec_effect(const std::shared_ptr<const core::Numerical>& numerical) {
    return m_caches.m_effect_cache->insert(std::make_unique<DecrementNumericalEffect>(numerical, m_caches.m_effect_cache->size())).first;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_bot_effect(const std::shared_ptr<const core::Numerical>& numerical) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedNumericalEffect>(numerical, m_caches.m_effect_cache->size())).first;
}

std::shared_ptr<const Rule> PolicyFactoryImpl::make_rule(const Conditions& conditions, const Effects& effects) {
    return m_caches.m_rule_cache->insert(std::unique_ptr<Rule>(new Rule(conditions, effects, m_caches.m_rule_cache->size()))).first;
}

std::shared_ptr<const Policy> PolicyFactoryImpl::make_policy(
    const Rules& rules) {
    return m_caches.m_policy_cache->insert(std::unique_ptr<Policy>(new Policy(rules, m_caches.m_policy_cache->size()))).first;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize( Archive& ar, dlplan::policy::PolicyFactoryImpl& t, const unsigned int /* version */ )
{
    ar & t.m_caches;
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::PolicyFactoryImpl& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::PolicyFactoryImpl& t, const unsigned int version);

}