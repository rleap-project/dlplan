#include "../../include/dlplan/policy.h"

#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/weak_ptr.hpp>

#include <sstream>


namespace dlplan::policy {
NamedRole::NamedRole(int identifier, const std::string& key, std::shared_ptr<const core::Role> role)
    : m_identifier(identifier), m_key(key), m_role(role) { }

NamedRole::NamedRole(const NamedRole& other) = default;

NamedRole& NamedRole::operator=(const NamedRole& other) = default;

NamedRole::NamedRole(NamedRole&& other) = default;

NamedRole& NamedRole::operator=(NamedRole&& other) = default;

NamedRole::~NamedRole() = default;

bool NamedRole::operator==(const NamedRole& other) const {
    if (this != &other) {
        return m_key == other.m_key
            && m_role == other.m_role;
    }
    return true;
}

bool NamedRole::operator<(const NamedRole& other) const {
    return m_identifier < other.m_identifier;
}

size_t NamedRole::hash() const {
    return hash_combine(m_key, m_role);
}

int NamedRole::compute_evaluate_time_score() const {
    return m_role->compute_evaluate_time_score();
}

std::string NamedRole::compute_repr() const {
    return get_key();
}

std::string NamedRole::str() const {
    return get_key();
}

const std::string& NamedRole::get_key() const {
    return m_key;
}

std::shared_ptr<const core::Role> NamedRole::get_role() const {
    return m_role;
}

}
