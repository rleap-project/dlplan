#include "../../../../include/dlplan/core/elements/concepts/all.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/weak_ptr.hpp>


namespace dlplan::core {
void AllConcept::compute_result(const RoleDenotation& role_denot, const ConceptDenotation& concept_denot, ConceptDenotation& result) const {
    // find counterexamples b : exists b . (a,b) in R and b notin C
    result.set();
    for (const auto& pair : role_denot.to_vector()) {
        if (!concept_denot.contains(pair.second)) {
            result.erase(pair.first);
        }
    }
}

ConceptDenotation AllConcept::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_role->evaluate(state, caches),
        *m_concept->evaluate(state, caches),
        denotation);
    return denotation;
}

ConceptDenotations AllConcept::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    ConceptDenotations denotations;
    denotations.reserve(states.size());
    auto role_denotations = m_role->evaluate(states, caches);
    auto concept_denotations = m_concept->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*role_denotations)[i],
            *(*concept_denotations)[i],
            denotation);
        denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
    }
    return denotations;
}

AllConcept::AllConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role, std::shared_ptr<const Concept> concept)
    : Concept(vocabulary_info, index, role->is_static() && concept->is_static()), m_role(role), m_concept(concept) { }

bool AllConcept::operator==(const Concept& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const AllConcept&>(other);
        return m_is_static == other_derived.m_is_static
            && m_role == other_derived.m_role
            && m_concept == other_derived.m_concept;
    }
    return false;
}

size_t AllConcept::hash() const {
    return dlplan::utils::hash_combine(m_is_static, m_role, m_concept);
}

ConceptDenotation AllConcept::evaluate(const State& state) const {
    auto denotation = ConceptDenotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_role->evaluate(state),
        m_concept->evaluate(state),
        denotation);
    return denotation;
}

int AllConcept::compute_complexity() const {
    return m_role->compute_complexity() + m_concept->compute_complexity() + 1;
}

void AllConcept::compute_repr(std::stringstream& out) const {
    out << "c_all" << "(";
    m_role->compute_repr(out);
    out << ",";
    m_concept->compute_repr(out);
    out << ")";
}

int AllConcept::compute_evaluate_time_score() const {
    return m_role->compute_evaluate_time_score() + m_concept->compute_evaluate_time_score() + SCORE_QUADRATIC;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::AllConcept& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Concept>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::AllConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role;
    ar << t->m_concept;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::AllConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Concept> concept_;
    std::shared_ptr<const dlplan::core::Role> role;
    ar >> vocabulary;
    ar >> index;
    ar >> role;
    ar >> concept_;
    ::new(t)dlplan::core::AllConcept(index, vocabulary, role, concept_);
}


template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::AllConcept, std::weak_ptr<dlplan::core::AllConcept>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::AllConcept, std::weak_ptr<dlplan::core::AllConcept>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::AllConcept, std::weak_ptr<dlplan::core::AllConcept>>* t, const unsigned int /*version*/) {
    dlplan::core::AllConcept* first = nullptr;
    std::weak_ptr<dlplan::core::AllConcept>* second = nullptr;
    ar >> const_cast<dlplan::core::AllConcept&>(*first);
    ar >> second;
    ::new(t)std::pair<const dlplan::core::AllConcept, std::weak_ptr<dlplan::core::AllConcept>>(*first, *second);
    delete first;
    delete second;
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::AllConcept& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::AllConcept& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::core::AllConcept* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::core::AllConcept* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::core::AllConcept, std::weak_ptr<dlplan::core::AllConcept>>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    std::pair<const dlplan::core::AllConcept, std::weak_ptr<dlplan::core::AllConcept>>& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const std::pair<const dlplan::core::AllConcept, std::weak_ptr<dlplan::core::AllConcept>>* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::core::AllConcept, std::weak_ptr<dlplan::core::AllConcept>>* t, const unsigned int version);
}

BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::core::AllConcept)


namespace std {
    bool less<std::shared_ptr<const dlplan::core::AllConcept>>::operator()(
        const std::shared_ptr<const dlplan::core::AllConcept>& left_concept,
        const std::shared_ptr<const dlplan::core::AllConcept>& right_concept) const {
        return *left_concept < *right_concept;
    }

    std::size_t hash<dlplan::core::AllConcept>::operator()(const dlplan::core::AllConcept& concept_) const {
        return concept_.hash();
    }
}