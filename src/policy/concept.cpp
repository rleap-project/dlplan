#include "../../include/dlplan/policy.h"

#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"

#include <sstream>


namespace dlplan::policy {
NamedConcept::NamedConcept(int identifier, const std::string& key, std::shared_ptr<const core::Concept> concept)
    : m_identifier(identifier), m_key(key), m_concept(concept) { }

NamedConcept::NamedConcept(const NamedConcept& other) = default;

NamedConcept& NamedConcept::operator=(const NamedConcept& other) = default;

NamedConcept::NamedConcept(NamedConcept&& other) = default;

NamedConcept& NamedConcept::operator=(NamedConcept&& other) = default;

NamedConcept::~NamedConcept() = default;

bool NamedConcept::operator==(const NamedConcept& other) const {
    if (this != &other) {
        return m_key == other.m_key
            && m_concept == other.m_concept;
    }
    return true;
}

bool NamedConcept::operator<(const NamedConcept& other) const {
    return m_identifier < other.m_identifier;
}

size_t NamedConcept::hash() const {
    return hash_combine(m_key, m_concept);
}

int NamedConcept::compute_evaluate_time_score() const {
    return m_concept->compute_evaluate_time_score();
}

std::string NamedConcept::compute_repr() const {
    return get_key();
}

std::string NamedConcept::str() const {
    return get_key();
}

const std::string& NamedConcept::get_key() const {
    return m_key;
}

std::shared_ptr<const core::Concept> NamedConcept::get_concept() const {
    return m_concept;
}

}
