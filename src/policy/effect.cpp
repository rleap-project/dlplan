#include "effect.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "include/dlplan/core.h"

using namespace dlplan;


namespace dlplan::policy {

BooleanEffect::BooleanEffect(std::shared_ptr<const core::Boolean> boolean, EffectIndex index)
    : BaseEffect(index), m_boolean(boolean) { }

int BooleanEffect::compute_evaluate_time_score() const {
    return m_boolean->compute_evaluate_time_score();
}

std::shared_ptr<const core::Boolean> BooleanEffect::get_boolean() const {
    return m_boolean;
}

std::shared_ptr<const core::Numerical> BooleanEffect::get_numerical() const {
    return nullptr;
}


NumericalEffect::NumericalEffect(std::shared_ptr<const core::Numerical> numerical, EffectIndex index)
    : BaseEffect(index), m_numerical(numerical) { }

int NumericalEffect::compute_evaluate_time_score() const {
    return m_numerical->compute_evaluate_time_score();
}

std::shared_ptr<const core::Boolean> NumericalEffect::get_boolean() const {
    return nullptr;
}

std::shared_ptr<const core::Numerical> NumericalEffect::get_numerical() const {
    return m_numerical;
}


PositiveBooleanEffect::PositiveBooleanEffect(std::shared_ptr<const core::Boolean> boolean, EffectIndex index)
    : BooleanEffect(boolean, index) {}

bool PositiveBooleanEffect::evaluate(const core::State&, const core::State& target_state) const {
    return m_boolean->evaluate(target_state);
}

bool PositiveBooleanEffect::evaluate(const core::State&, const core::State& target_state, core::DenotationsCaches& caches) const {
    return m_boolean->evaluate(target_state, caches);
}

std::string PositiveBooleanEffect::compute_repr() const{
    return "(:e_b_pos \"" + m_boolean->compute_repr() + "\")";
}

std::string PositiveBooleanEffect::str() const {
    return "(:e_b_pos " + std::string("b") + std::to_string(m_boolean->get_index()) + ")";
}


NegativeBooleanEffect::NegativeBooleanEffect(std::shared_ptr<const core::Boolean> boolean, EffectIndex index)
    : BooleanEffect(boolean, index) {}

bool NegativeBooleanEffect::evaluate(const core::State&, const core::State& target_state) const {
    return !m_boolean->evaluate(target_state);
}

bool NegativeBooleanEffect::evaluate(const core::State&, const core::State& target_state, core::DenotationsCaches& caches) const {
    return !m_boolean->evaluate(target_state, caches);
}

std::string NegativeBooleanEffect::compute_repr() const{
    return "(:e_b_neg \"" + m_boolean->compute_repr() + "\")";
}

std::string NegativeBooleanEffect::str() const {
    return "(:e_b_neg " + std::string("b") + std::to_string(m_boolean->get_index()) + ")";
}


UnchangedBooleanEffect::UnchangedBooleanEffect(std::shared_ptr<const core::Boolean> boolean, EffectIndex index)
    : BooleanEffect(boolean, index) {}

bool UnchangedBooleanEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    return m_boolean->evaluate(source_state) == m_boolean->evaluate(target_state);
}

bool UnchangedBooleanEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    return m_boolean->evaluate(source_state, caches) == m_boolean->evaluate(target_state, caches);
}

std::string UnchangedBooleanEffect::compute_repr() const{
    return "(:e_b_bot \"" + m_boolean->compute_repr() + "\")";
}

std::string UnchangedBooleanEffect::str() const {
    return "(:e_b_bot " + std::string("b") + std::to_string(m_boolean->get_index()) + ")";
}


IncrementNumericalEffect::IncrementNumericalEffect(std::shared_ptr<const core::Numerical> numerical, EffectIndex index)
    : NumericalEffect(numerical, index) {}

bool IncrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    return m_numerical->evaluate(source_state) < m_numerical->evaluate(target_state);
}

bool IncrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    return m_numerical->evaluate(source_state, caches) < m_numerical->evaluate(target_state, caches);
}

std::string IncrementNumericalEffect::compute_repr() const{
    return "(:e_n_inc \"" + m_numerical->compute_repr() + "\")";
}

std::string IncrementNumericalEffect::str() const {
    return "(:e_n_inc " + std::string("n") + std::to_string(m_numerical->get_index()) + ")";
}


DecrementNumericalEffect::DecrementNumericalEffect(std::shared_ptr<const core::Numerical> numerical, EffectIndex index)
    : NumericalEffect(numerical, index) {}

bool DecrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    return m_numerical->evaluate(source_state) > m_numerical->evaluate(target_state);
}

bool DecrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    return m_numerical->evaluate(source_state, caches) > m_numerical->evaluate(target_state, caches);
}

std::string DecrementNumericalEffect::compute_repr() const{
    return "(:e_n_dec \"" + m_numerical->compute_repr() + "\")";
}

std::string DecrementNumericalEffect::str() const {
    return "(:e_n_dec " + std::string("n") + std::to_string(m_numerical->get_index()) + ")";
}


UnchangedNumericalEffect::UnchangedNumericalEffect(std::shared_ptr<const core::Numerical> numerical, EffectIndex index)
    : NumericalEffect(numerical, index) {}

bool UnchangedNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    return m_numerical->evaluate(source_state) == m_numerical->evaluate(target_state);
}

bool UnchangedNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    return m_numerical->evaluate(source_state, caches) == m_numerical->evaluate(target_state, caches);
}

std::string UnchangedNumericalEffect::compute_repr() const{
    return "(:e_n_bot \"" + m_numerical->compute_repr() + "\")";
}

std::string UnchangedNumericalEffect::str() const {
    return "(:e_n_bot " + std::string("n") + std::to_string(m_numerical->get_index()) + ")";
}

}

namespace boost::serialization {
template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::policy::BooleanEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::BaseEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& /* ar */ , const dlplan::policy::BooleanEffect* /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void load_construct_data(Archive& /* ar */ , dlplan::policy::BooleanEffect* /* t */ , const unsigned int /* version */ )
{
}

template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::policy::NumericalEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::BaseEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& /* ar */ , const dlplan::policy::NumericalEffect* /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void load_construct_data(Archive& /* ar */ , dlplan::policy::NumericalEffect* /* t */ , const unsigned int /* version */ )
{
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::PositiveBooleanEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::BooleanEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::PositiveBooleanEffect* t, const unsigned int /* version */ )
{
    ar << t->m_boolean;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::PositiveBooleanEffect* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::Boolean> boolean;
    dlplan::policy::EffectIndex index;
    ar >> boolean;
    ar >> index;
    ::new(t)dlplan::policy::PositiveBooleanEffect(boolean, index);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::NegativeBooleanEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::BooleanEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::NegativeBooleanEffect* t, const unsigned int /* version */ )
{
    ar << t->m_boolean;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::NegativeBooleanEffect* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::Boolean> boolean;
    dlplan::policy::EffectIndex index;
    ar >> boolean;
    ar >> index;
    ::new(t)dlplan::policy::NegativeBooleanEffect(boolean, index);
}

template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::policy::UnchangedBooleanEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::BooleanEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::UnchangedBooleanEffect* t, const unsigned int /* version */ )
{
    ar << t->m_boolean;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::UnchangedBooleanEffect* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::Boolean> boolean;
    dlplan::policy::EffectIndex index;
    ar >> boolean;
    ar >> index;
    ::new(t)dlplan::policy::UnchangedBooleanEffect(boolean, index);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::IncrementNumericalEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::NumericalEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::IncrementNumericalEffect* t, const unsigned int /* version */ )
{
    ar << t->m_numerical;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::IncrementNumericalEffect* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::Numerical> numerical;
    dlplan::policy::EffectIndex index;
    ar >> numerical;
    ar >> index;
    ::new(t)dlplan::policy::IncrementNumericalEffect(numerical, index);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::DecrementNumericalEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::NumericalEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::DecrementNumericalEffect* t, const unsigned int /* version */ )
{
    ar << t->m_numerical;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::DecrementNumericalEffect* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::Numerical> numerical;
    dlplan::policy::EffectIndex index;
    ar >> numerical;
    ar >> index;
    ::new(t)dlplan::policy::DecrementNumericalEffect(numerical, index);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::UnchangedNumericalEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::NumericalEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::UnchangedNumericalEffect* t, const unsigned int /* version */ )
{
    ar << t->m_numerical;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::UnchangedNumericalEffect* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::Numerical> numerical;
    dlplan::policy::EffectIndex index;
    ar >> numerical;
    ar >> index;
    ::new(t)dlplan::policy::UnchangedNumericalEffect(numerical, index);
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::BooleanEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::BooleanEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::BooleanEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::BooleanEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::PositiveBooleanEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::PositiveBooleanEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::PositiveBooleanEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::PositiveBooleanEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::NegativeBooleanEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::NegativeBooleanEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::NegativeBooleanEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::NegativeBooleanEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::UnchangedBooleanEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::UnchangedBooleanEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::UnchangedBooleanEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::UnchangedBooleanEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::NumericalEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::NumericalEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::NumericalEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::NumericalEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::IncrementNumericalEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::IncrementNumericalEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::IncrementNumericalEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::IncrementNumericalEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::DecrementNumericalEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::DecrementNumericalEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::DecrementNumericalEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::DecrementNumericalEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::UnchangedNumericalEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::UnchangedNumericalEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::UnchangedNumericalEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::UnchangedNumericalEffect* t, const unsigned int version);
}

BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::PositiveBooleanEffect)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::NegativeBooleanEffect)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::UnchangedBooleanEffect)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::IncrementNumericalEffect)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::DecrementNumericalEffect)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::UnchangedNumericalEffect)
