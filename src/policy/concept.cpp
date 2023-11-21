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
NamedConcept::NamedConcept() : m_identifier(-1), m_key(""), m_concept(nullptr) { }

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
}

size_t NamedConcept::hash() const {
    return dlplan::utils::hash_combine(m_key, m_concept);
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


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& ar, dlplan::policy::NamedConcept& t, const unsigned int /* version */ )
{
    ar & t.m_identifier;
    ar & t.m_key;
    ar & t.m_concept;
}

template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::policy::NamedConcept, std::weak_ptr<dlplan::policy::NamedConcept>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::policy::NamedConcept, std::weak_ptr<dlplan::policy::NamedConcept>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::policy::NamedConcept, std::weak_ptr<dlplan::policy::NamedConcept>>* t, const unsigned int /*version*/) {
    dlplan::policy::NamedConcept* first = nullptr;
    std::weak_ptr<dlplan::policy::NamedConcept>* second = nullptr;
    ar >> const_cast<dlplan::policy::NamedConcept&>(*first);
    ar >> second;
    ::new(t)std::pair<const dlplan::policy::NamedConcept, std::weak_ptr<dlplan::policy::NamedConcept>>(*first, *second);
    delete first;
    delete second;
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::NamedConcept& t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::policy::NamedConcept, std::weak_ptr<dlplan::policy::NamedConcept>>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    std::pair<const dlplan::policy::NamedConcept, std::weak_ptr<dlplan::policy::NamedConcept>>& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const std::pair<const dlplan::policy::NamedConcept, std::weak_ptr<dlplan::policy::NamedConcept>>* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::policy::NamedConcept, std::weak_ptr<dlplan::policy::NamedConcept>>* t, const unsigned int version);

}


