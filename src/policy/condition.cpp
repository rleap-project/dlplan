#include "condition.h"

#include "../core/elements/utils.h"
#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>

using namespace dlplan;


namespace dlplan::policy {

BooleanCondition::BooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean)
    : BaseCondition(identifier), m_boolean(boolean) { }

int BooleanCondition::compute_evaluate_time_score() const {
    return m_boolean->compute_evaluate_time_score();
}

std::shared_ptr<const NamedBoolean> BooleanCondition::get_boolean() const {
    return m_boolean;
}

std::shared_ptr<const NamedNumerical> BooleanCondition::get_numerical() const {
    return nullptr;
}


NumericalCondition::NumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : BaseCondition(identifier), m_numerical(numerical) { }

int NumericalCondition::compute_evaluate_time_score() const {
    return m_numerical->compute_evaluate_time_score();
}

std::shared_ptr<const NamedBoolean> NumericalCondition::get_boolean() const {
    return nullptr;
}

std::shared_ptr<const NamedNumerical> NumericalCondition::get_numerical() const {
    return m_numerical;
}


PositiveBooleanCondition::PositiveBooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean)
    : BooleanCondition(identifier, boolean) { }

bool PositiveBooleanCondition::operator==(const BaseCondition& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const PositiveBooleanCondition&>(other);
        return m_boolean == other_derived.m_boolean;
    }
    return false;
}

size_t PositiveBooleanCondition::hash() const {
    return dlplan::utils::hash_combine(m_boolean);
}

bool PositiveBooleanCondition::evaluate(const core::State& source_state) const {
    return m_boolean->get_boolean()->evaluate(source_state);
}

bool PositiveBooleanCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    return m_boolean->get_boolean()->evaluate(source_state, caches);
}

std::string PositiveBooleanCondition::compute_repr() const {
    return "(:c_b_pos \"" + m_boolean->get_boolean()->compute_repr() + "\")";
}

std::string PositiveBooleanCondition::str() const {
    return "(:c_b_pos " + m_boolean->get_key() + ")";
}


NegativeBooleanCondition::NegativeBooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean)
    : BooleanCondition(identifier, boolean) { }

bool NegativeBooleanCondition::operator==(const BaseCondition& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const NegativeBooleanCondition&>(other);
        return m_boolean == other_derived.m_boolean;
    }
    return false;
}

size_t NegativeBooleanCondition::hash() const {
    return dlplan::utils::hash_combine(m_boolean);
}

bool NegativeBooleanCondition::evaluate(const core::State& source_state) const {
    return !m_boolean->get_boolean()->evaluate(source_state);
}

bool NegativeBooleanCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    return !m_boolean->get_boolean()->evaluate(source_state, caches);
}

std::string NegativeBooleanCondition::compute_repr() const {
    return "(:c_b_neg \"" + m_boolean->get_boolean()->compute_repr() + "\")";
}

std::string NegativeBooleanCondition::str() const {
    return "(:c_b_neg " + m_boolean->get_key() + ")";
}


EqualNumericalCondition::EqualNumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NumericalCondition(identifier, numerical) { }

bool EqualNumericalCondition::operator==(const BaseCondition& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const EqualNumericalCondition&>(other);
        return m_numerical == other_derived.m_numerical;
    }
    return false;
}

size_t EqualNumericalCondition::hash() const {
    return dlplan::utils::hash_combine(m_numerical);
}

bool EqualNumericalCondition::evaluate(const core::State& source_state) const {
    int eval = m_numerical->get_numerical()->evaluate(source_state);
    if (eval == INF) return false;
    return eval == 0;
}

bool EqualNumericalCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    int eval = m_numerical->get_numerical()->evaluate(source_state, caches);
    if (eval == INF) return false;
    return eval == 0;
}

std::string EqualNumericalCondition::compute_repr() const {
    return "(:c_n_eq \"" + m_numerical->get_numerical()->compute_repr() + "\")";
}

std::string EqualNumericalCondition::str() const {
    return "(:c_n_eq " + m_numerical->get_key() + ")";
}


GreaterNumericalCondition::GreaterNumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NumericalCondition(identifier, numerical) { }

bool GreaterNumericalCondition::operator==(const BaseCondition& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const GreaterNumericalCondition&>(other);
        return m_numerical == other_derived.m_numerical;
    }
    return false;
}

size_t GreaterNumericalCondition::hash() const {
    return dlplan::utils::hash_combine(m_numerical);
}

bool GreaterNumericalCondition::evaluate(const core::State& source_state) const {
    int eval = m_numerical->get_numerical()->evaluate(source_state);
    if (eval == INF) return false;
    return eval > 0;
}

bool GreaterNumericalCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    int eval = m_numerical->get_numerical()->evaluate(source_state, caches);
    if (eval == INF) return false;
    return eval > 0;
}

std::string GreaterNumericalCondition::compute_repr() const {
    return "(:c_n_gt \"" + m_numerical->get_numerical()->compute_repr() + "\")";
}

std::string GreaterNumericalCondition::str() const {
    return "(:c_n_gt " + m_numerical->get_key() + ")";
}

}


namespace boost::serialization {

template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::policy::BooleanCondition& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::BaseCondition>(t);
}

template<class Archive>
void save_construct_data(Archive& /* ar */ , const dlplan::policy::BooleanCondition* /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void load_construct_data(Archive& /* ar */ , dlplan::policy::BooleanCondition* /* t */ , const unsigned int /* version */ )
{
}

template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::policy::NumericalCondition& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::BaseCondition>(t);
}

template<class Archive>
void save_construct_data(Archive& /* ar */ , const dlplan::policy::NumericalCondition* /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void load_construct_data(Archive& /* ar */ , dlplan::policy::NumericalCondition* /* t */ , const unsigned int /* version */ )
{
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::PositiveBooleanCondition& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::BooleanCondition>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::PositiveBooleanCondition* t, const unsigned int /* version */ )
{
    ar << t->m_identifier;
    ar << t->m_boolean;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::PositiveBooleanCondition* t, const unsigned int /* version */ )
{
    int identifier;
    std::shared_ptr<const dlplan::policy::NamedBoolean> boolean;
    ar >> identifier;
    ar >> boolean;
    ::new(t)dlplan::policy::PositiveBooleanCondition(identifier, boolean);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::NegativeBooleanCondition& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::BooleanCondition>(t);
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::policy::NegativeBooleanCondition* t, const unsigned int /* version */ )
{
    ar << t->m_identifier;
    ar << t->m_boolean;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::policy::NegativeBooleanCondition* t, const unsigned int /* version */ )
{
    int identifier;
    std::shared_ptr<const dlplan::policy::NamedBoolean> boolean;
    ar >> identifier;
    ar >> boolean;
    ::new(t)dlplan::policy::NegativeBooleanCondition(identifier, boolean);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::GreaterNumericalCondition& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::NumericalCondition>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::GreaterNumericalCondition* t, const unsigned int /* version */ )
{
    ar << t->m_identifier;
    ar << t->m_numerical;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::GreaterNumericalCondition* t, const unsigned int /* version */ )
{
    int identifier;
    std::shared_ptr<const dlplan::policy::NamedNumerical> numerical;
    ar >> identifier;
    ar >> numerical;
    ::new(t)dlplan::policy::GreaterNumericalCondition(identifier, numerical);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::EqualNumericalCondition& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::NumericalCondition>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::EqualNumericalCondition* t, const unsigned int /* version */ )
{
    ar << t->m_identifier;
    ar << t->m_numerical;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::EqualNumericalCondition* t, const unsigned int /* version */ )
{
    int identifier;
    std::shared_ptr<const dlplan::policy::NamedNumerical> numerical;
    ar >> identifier;
    ar >> numerical;
    ::new(t)dlplan::policy::EqualNumericalCondition(identifier, numerical);
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::BooleanCondition& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::BooleanCondition& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::BooleanCondition* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::BooleanCondition* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::PositiveBooleanCondition& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::PositiveBooleanCondition& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::PositiveBooleanCondition* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::PositiveBooleanCondition* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::NegativeBooleanCondition& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::NegativeBooleanCondition& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::NegativeBooleanCondition* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::NegativeBooleanCondition* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::NumericalCondition& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::NumericalCondition& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::NumericalCondition* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::NumericalCondition* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::GreaterNumericalCondition& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::GreaterNumericalCondition& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::GreaterNumericalCondition* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::GreaterNumericalCondition* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::EqualNumericalCondition& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::EqualNumericalCondition& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::EqualNumericalCondition* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::EqualNumericalCondition* t, const unsigned int version);
}

BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::PositiveBooleanCondition)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::NegativeBooleanCondition)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::GreaterNumericalCondition)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::EqualNumericalCondition)
