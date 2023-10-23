#include "condition.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "include/dlplan/core.h"

using namespace dlplan;


namespace dlplan::policy {

BooleanCondition::BooleanCondition(std::shared_ptr<const NamedBoolean> boolean, ConditionIndex index)
    : BaseCondition(index), m_boolean(boolean) { }

int BooleanCondition::compute_evaluate_time_score() const {
    return m_boolean->compute_evaluate_time_score();
}

std::shared_ptr<const NamedBoolean> BooleanCondition::get_boolean() const {
    return m_boolean;
}

std::shared_ptr<const NamedNumerical> BooleanCondition::get_numerical() const {
    return nullptr;
}


NumericalCondition::NumericalCondition(std::shared_ptr<const NamedNumerical> numerical, ConditionIndex index)
    : BaseCondition(index), m_numerical(numerical) { }

int NumericalCondition::compute_evaluate_time_score() const {
    return m_numerical->compute_evaluate_time_score();
}

std::shared_ptr<const NamedBoolean> NumericalCondition::get_boolean() const {
    return nullptr;
}

std::shared_ptr<const NamedNumerical> NumericalCondition::get_numerical() const {
    return m_numerical;
}


PositiveBooleanCondition::PositiveBooleanCondition(std::shared_ptr<const NamedBoolean> boolean, ConditionIndex index)
    : BooleanCondition(boolean, index) { }

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


NegativeBooleanCondition::NegativeBooleanCondition(std::shared_ptr<const NamedBoolean> boolean, ConditionIndex index)
    : BooleanCondition(boolean, index) { }

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


EqualNumericalCondition::EqualNumericalCondition(std::shared_ptr<const NamedNumerical> numerical, ConditionIndex index)
    : NumericalCondition(numerical, index) { }

bool EqualNumericalCondition::evaluate(const core::State& source_state) const {
    return m_numerical->get_numerical()->evaluate(source_state) == 0;
}

bool EqualNumericalCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    return m_numerical->get_numerical()->evaluate(source_state, caches) == 0;
}

std::string EqualNumericalCondition::compute_repr() const {
    return "(:c_n_eq \"" + m_numerical->get_numerical()->compute_repr() + "\")";
}

std::string EqualNumericalCondition::str() const {
    return "(:c_n_eq " + m_numerical->get_key() + ")";
}


GreaterNumericalCondition::GreaterNumericalCondition(std::shared_ptr<const NamedNumerical> numerical, ConditionIndex index)
    : NumericalCondition(numerical, index) { }

bool GreaterNumericalCondition::evaluate(const core::State& source_state) const {
    return m_numerical->get_numerical()->evaluate(source_state) > 0;
}

bool GreaterNumericalCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    return m_numerical->get_numerical()->evaluate(source_state, caches) > 0;
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
    ar << t->m_boolean;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::PositiveBooleanCondition* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::policy::NamedBoolean> boolean;
    dlplan::policy::ConditionIndex index;
    ar >> boolean;
    ar >> index;
    ::new(t)dlplan::policy::PositiveBooleanCondition(boolean, index);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::NegativeBooleanCondition& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::BooleanCondition>(t);
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::policy::NegativeBooleanCondition* t, const unsigned int /* version */ )
{
    ar << t->m_boolean;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::policy::NegativeBooleanCondition* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::policy::NamedBoolean> boolean;
    dlplan::policy::ConditionIndex index;
    ar >> boolean;
    ar >> index;
    ::new(t)dlplan::policy::NegativeBooleanCondition(boolean, index);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::GreaterNumericalCondition& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::NumericalCondition>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::GreaterNumericalCondition* t, const unsigned int /* version */ )
{
    ar << t->m_numerical;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::GreaterNumericalCondition* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::policy::NamedNumerical> numerical;
    dlplan::policy::ConditionIndex index;
    ar >> numerical;
    ar >> index;
    ::new(t)dlplan::policy::GreaterNumericalCondition(numerical, index);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::EqualNumericalCondition& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::NumericalCondition>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::EqualNumericalCondition* t, const unsigned int /* version */ )
{
    ar << t->m_numerical;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::EqualNumericalCondition* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::policy::NamedNumerical> numerical;
    dlplan::policy::ConditionIndex index;
    ar >> numerical;
    ar >> index;
    ::new(t)dlplan::policy::EqualNumericalCondition(numerical, index);
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
