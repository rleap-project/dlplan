#include "../../include/dlplan/policy.h"

#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <sstream>


namespace dlplan::policy {
NamedNumerical::NamedNumerical() : m_identifier(-1), m_key(""), m_numerical(nullptr) { }

NamedNumerical::NamedNumerical(int identifier, const std::string& key, std::shared_ptr<const core::Numerical> numerical)
    : m_identifier(identifier), m_key(key), m_numerical(numerical) { }

NamedNumerical::NamedNumerical(const NamedNumerical& other) = default;

NamedNumerical& NamedNumerical::operator=(const NamedNumerical& other) = default;

NamedNumerical::NamedNumerical(NamedNumerical&& other) = default;

NamedNumerical& NamedNumerical::operator=(NamedNumerical&& other) = default;

NamedNumerical::~NamedNumerical() = default;

bool NamedNumerical::operator==(const NamedNumerical& other) const {
    if (this != &other) {
        return m_key == other.m_key
            && m_numerical == other.m_numerical;
    }
    return true;
}
bool NamedNumerical::operator<(const NamedNumerical& other) const {
}

size_t NamedNumerical::hash() const {
    return dlplan::utils::hash_combine(m_key, m_numerical);
}

int NamedNumerical::compute_evaluate_time_score() const {
    return m_numerical->compute_evaluate_time_score();
}

std::string NamedNumerical::compute_repr() const {
    return get_key();
}

std::string NamedNumerical::str() const {
    return get_key();
}

const std::string& NamedNumerical::get_key() const {
    return m_key;
}

std::shared_ptr<const core::Numerical> NamedNumerical::get_numerical() const {
    return m_numerical;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& ar, dlplan::policy::NamedNumerical& t, const unsigned int /* version */ )
{
    ar & t.m_identifier;
    ar & t.m_key;
    ar & t.m_numerical;
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
