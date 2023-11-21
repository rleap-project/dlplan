#include "../../include/dlplan/policy.h"

#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"

#include <sstream>


namespace dlplan::policy {
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
    return m_identifier < other.m_identifier;
}

size_t NamedBoolean::hash() const {
    return hash_combine(m_key, m_boolean);
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
