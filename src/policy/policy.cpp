#include "include/dlplan/policy.h"

#include <iostream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "condition.h"
#include "effect.h"
#include "policy_factory.h"
#include "include/dlplan/policy.h"


using namespace dlplan::common::parsers;


namespace dlplan::policy {


BaseCondition::BaseCondition(ConditionIndex index) : m_index(index) { }

BaseCondition::~BaseCondition() = default;

ConditionIndex BaseCondition::get_index() const {
    return m_index;
}


BaseEffect::BaseEffect(EffectIndex index) : m_index(index) { }

BaseEffect::~BaseEffect() = default;

EffectIndex BaseEffect::get_index() const {
    return m_index;
}


PolicyFactory::PolicyFactory() : m_pImpl(nullptr) { }

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

std::shared_ptr<const BaseCondition> PolicyFactory::make_pos_condition(const std::shared_ptr<const core::Boolean>& boolean) {
    return m_pImpl->make_pos_condition(boolean);
}

std::shared_ptr<const BaseCondition> PolicyFactory::make_neg_condition(const std::shared_ptr<const core::Boolean>& boolean) {
    return m_pImpl->make_neg_condition(boolean);
}

std::shared_ptr<const BaseCondition> PolicyFactory::make_gt_condition(const std::shared_ptr<const core::Numerical>& numerical) {
    return m_pImpl->make_gt_condition(numerical);
}

std::shared_ptr<const BaseCondition> PolicyFactory::make_eq_condition(const std::shared_ptr<const core::Numerical>& numerical) {
    return m_pImpl->make_eq_condition(numerical);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_pos_effect(const std::shared_ptr<const core::Boolean>& boolean) {
    return m_pImpl->make_pos_effect(boolean);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_neg_effect(const std::shared_ptr<const core::Boolean>& boolean) {
    return m_pImpl->make_neg_effect(boolean);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_bot_effect(const std::shared_ptr<const core::Boolean>& boolean) {
    return m_pImpl->make_bot_effect(boolean);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_inc_effect(const std::shared_ptr<const core::Numerical>& numerical) {
    return m_pImpl->make_inc_effect(numerical);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_dec_effect(const std::shared_ptr<const core::Numerical>& numerical) {
    return m_pImpl->make_dec_effect(numerical);
}

std::shared_ptr<const BaseEffect> PolicyFactory::make_bot_effect(const std::shared_ptr<const core::Numerical>& numerical) {
    return m_pImpl->make_bot_effect(numerical);
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


namespace boost::serialization {
template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::policy::BaseCondition& /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive& /* ar */ , const dlplan::policy::BaseCondition* /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void load_construct_data(Archive& /* ar */ , dlplan::policy::BaseCondition* /* t */ , const unsigned int /* version */ )
{
}

template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::policy::BaseEffect& /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive& /* ar */ , const dlplan::policy::BaseEffect* /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void load_construct_data(Archive& /* ar */ , dlplan::policy::BaseEffect* /* t */ , const unsigned int /* version */ )
{
}

template<typename Archive>
void serialize( Archive& ar, dlplan::policy::PolicyFactory& t, const unsigned int /* version */ )
{
    ar & t.m_pImpl;
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::BaseCondition& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::BaseCondition& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::BaseCondition* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::BaseCondition* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::BaseEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::BaseEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::BaseEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::BaseEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::PolicyFactory& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::PolicyFactory& t, const unsigned int version);
}
