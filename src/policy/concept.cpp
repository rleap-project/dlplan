#include "include/dlplan/policy.h"

#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "include/dlplan/core.h"


namespace dlplan::policy {

NamedConcept::NamedConcept(const std::string& key, std::shared_ptr<const core::Concept> concept)
    : NamedBaseElement(key), m_concept(concept) { }

NamedConcept::NamedConcept(const NamedConcept& other) = default;

NamedConcept& NamedConcept::operator=(const NamedConcept& other) = default;

NamedConcept::NamedConcept(NamedConcept&& other) = default;

NamedConcept& NamedConcept::operator=(NamedConcept&& other) = default;

NamedConcept::~NamedConcept() = default;

int NamedConcept::compute_evaluate_time_score() const {
    return m_concept->compute_evaluate_time_score();
}

std::string NamedConcept::compute_repr() const {
    return get_key();
}

std::string NamedConcept::str() const {
    return get_key();
}

std::shared_ptr<const core::Concept> NamedConcept::get_concept() const {
    return m_concept;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */, dlplan::policy::NamedConcept& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::NamedBaseElement>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::NamedConcept* t, const unsigned int /* version */ )
{
    ar << t->m_key;
    ar << t->m_concept;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::NamedConcept* t, const unsigned int /* version */ )
{
    std::string key;
    std::shared_ptr<const dlplan::core::Concept> concept;
    ar >> key;
    ar >> concept;
    ::new(t)dlplan::policy::NamedConcept(key, concept);
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::NamedConcept& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::NamedConcept& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::NamedConcept* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::NamedConcept* t, const unsigned int version);

}


