#include "include/dlplan/policy.h"

#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "include/dlplan/core.h"


namespace dlplan::policy {

NamedRole::NamedRole(const std::string& key, std::shared_ptr<const core::Role> role)
    : NamedBaseElement(key), m_role(role) { }

NamedRole::NamedRole(const NamedRole& other) = default;

NamedRole& NamedRole::operator=(const NamedRole& other) = default;

NamedRole::NamedRole(NamedRole&& other) = default;

NamedRole& NamedRole::operator=(NamedRole&& other) = default;

NamedRole::~NamedRole() = default;

int NamedRole::compute_evaluate_time_score() const {
    return m_role->compute_evaluate_time_score();
}

std::string NamedRole::compute_repr() const {
    return get_key();
}

std::string NamedRole::str() const {
    return get_key();
}

std::shared_ptr<const core::Role> NamedRole::get_role() const {
    return m_role;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */, dlplan::policy::NamedRole& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::NamedBaseElement>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::NamedRole* t, const unsigned int /* version */ )
{
    ar << t->m_key;
    ar << t->m_role;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::NamedRole* t, const unsigned int /* version */ )
{
    std::string key;
    std::shared_ptr<const dlplan::core::Role> role;
    ar >> key;
    ar >> role;
    ::new(t)dlplan::policy::NamedRole(key, role);
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::NamedRole& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::NamedRole& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::NamedRole* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::NamedRole* t, const unsigned int version);

}


