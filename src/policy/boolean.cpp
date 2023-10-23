#include "include/dlplan/policy.h"

#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "include/dlplan/core.h"


namespace dlplan::policy {

NamedBoolean::NamedBoolean(const std::string& key, std::shared_ptr<const core::Boolean> boolean)
    : NamedBaseElement(key), m_boolean(boolean) { }

NamedBoolean::NamedBoolean(const NamedBoolean& other) = default;

NamedBoolean& NamedBoolean::operator=(const NamedBoolean& other) = default;

NamedBoolean::NamedBoolean(NamedBoolean&& other) = default;

NamedBoolean& NamedBoolean::operator=(NamedBoolean&& other) = default;

NamedBoolean::~NamedBoolean() = default;

int NamedBoolean::compute_evaluate_time_score() const {
    return m_boolean->compute_evaluate_time_score();
}

std::string NamedBoolean::compute_repr() const {
    return get_key();
}

std::string NamedBoolean::str() const {
    return get_key();
}

std::shared_ptr<const core::Boolean> NamedBoolean::get_boolean() const {
    return m_boolean;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */, dlplan::policy::NamedBoolean& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::NamedBaseElement>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::NamedBoolean* t, const unsigned int /* version */ )
{
    ar << t->m_key;
    ar << t->m_boolean;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::NamedBoolean* t, const unsigned int /* version */ )
{
    std::string key;
    std::shared_ptr<const dlplan::core::Boolean> boolean;
    ar >> key;
    ar >> boolean;
    ::new(t)dlplan::policy::NamedBoolean(key, boolean);
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::NamedBoolean& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::NamedBoolean& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::NamedBoolean* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::NamedBoolean* t, const unsigned int version);

}


