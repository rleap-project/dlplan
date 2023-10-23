#include "include/dlplan/policy.h"

#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "include/dlplan/core.h"


namespace dlplan::policy {

NamedNumerical::NamedNumerical(const std::string& key, std::shared_ptr<const core::Numerical> numerical)
    : NamedBaseElement(key), m_numerical(numerical) { }

NamedNumerical::NamedNumerical(const NamedNumerical& other) = default;

NamedNumerical& NamedNumerical::operator=(const NamedNumerical& other) = default;

NamedNumerical::NamedNumerical(NamedNumerical&& other) = default;

NamedNumerical& NamedNumerical::operator=(NamedNumerical&& other) = default;

NamedNumerical::~NamedNumerical() = default;

int NamedNumerical::compute_evaluate_time_score() const {
    return m_numerical->compute_evaluate_time_score();
}

std::string NamedNumerical::compute_repr() const {
    return get_key();
}

std::string NamedNumerical::str() const {
    return get_key();
}

std::shared_ptr<const core::Numerical> NamedNumerical::get_numerical() const {
    return m_numerical;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */, dlplan::policy::NamedNumerical& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::NamedBaseElement>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::NamedNumerical* t, const unsigned int /* version */ )
{
    ar << t->m_key;
    ar << t->m_numerical;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::NamedNumerical* t, const unsigned int /* version */ )
{
    std::string key;
    std::shared_ptr<const dlplan::core::Numerical> numerical;
    ar >> key;
    ar >> numerical;
    ::new(t)dlplan::policy::NamedNumerical(key, numerical);
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::NamedNumerical& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::NamedNumerical& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::NamedNumerical* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::NamedNumerical* t, const unsigned int version);
}
