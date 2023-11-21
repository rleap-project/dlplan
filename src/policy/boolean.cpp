#include "../../include/dlplan/policy.h"

#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <sstream>


namespace dlplan::policy {
NamedBoolean::NamedBoolean() : m_identifier(-1), m_key(""), m_boolean(nullptr) { }

NamedBoolean::NamedBoolean(int identifier, const std::string& key, std::shared_ptr<const core::Boolean> boolean)
    : m_identifier(identifier), m_key(key), m_boolean(boolean) { }

NamedBoolean::NamedBoolean(const NamedBoolean& other) = default;

NamedBoolean& NamedBoolean::operator=(const NamedBoolean& other) = default;

NamedBoolean::NamedBoolean(NamedBoolean&& other) = default;

NamedBoolean& NamedBoolean::operator=(NamedBoolean&& other) = default;

NamedBoolean::~NamedBoolean() = default;

bool NamedBoolean::operator==(const NamedBoolean& other) const {
    if (this != &other) {
        return m_key == other.m_key
            && m_boolean == other.m_boolean;
    }
    return true;
}
bool NamedBoolean::operator<(const NamedBoolean& other) const {
}

size_t NamedBoolean::hash() const {
    return dlplan::utils::hash_combine(m_key, m_boolean);
}

int NamedBoolean::compute_evaluate_time_score() const {
    return m_boolean->compute_evaluate_time_score();
}

std::string NamedBoolean::compute_repr() const {
    return get_key();
}

std::string NamedBoolean::str() const {
    return get_key();
}

const std::string& NamedBoolean::get_key() const {
    return m_key;
}

std::shared_ptr<const core::Boolean> NamedBoolean::get_boolean() const {
    return m_boolean;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& ar, dlplan::policy::NamedBoolean& t, const unsigned int /* version */ )
{
    ar & t.m_identifier;
    ar & t.m_key;
    ar & t.m_boolean;
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


