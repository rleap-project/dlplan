#include "../../../../include/dlplan/core/elements/concepts/and.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/weak_ptr.hpp>


namespace dlplan::core {
void AndConcept::compute_result(const ConceptDenotation& left_denot, const ConceptDenotation& right_denot, ConceptDenotation& result) const {
    result = left_denot;
    result &= right_denot;
}

ConceptDenotation AndConcept::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    denotation.set();
    compute_result(
        *m_concept_left->evaluate(state, caches),
        *m_concept_right->evaluate(state, caches),
        denotation);
    return denotation;
}

ConceptDenotations AndConcept::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    ConceptDenotations denotations;
    denotations.reserve(states.size());
    auto concept_left_denotations = m_concept_left->evaluate(states, caches);
    auto concept_right_denotations = m_concept_right->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*concept_left_denotations)[i],
            *(*concept_right_denotations)[i],
            denotation);
        denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
    }
    return denotations;
}

AndConcept::AndConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Concept> concept_1, std::shared_ptr<const Concept> concept_2)
    : Concept(vocabulary_info, index, concept_1->is_static() && concept_2->is_static()),
    m_concept_left(concept_1->get_index() < concept_2->get_index() ? concept_1 : concept_2),
    m_concept_right(concept_1->get_index() < concept_2->get_index() ? concept_2 : concept_1) { }

bool AndConcept::operator==(const Concept& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const AndConcept&>(other);
        return m_is_static == other_derived.m_is_static
            && m_concept_left == other_derived.m_concept_left
            && m_concept_right == other_derived.m_concept_right;
    }
    return false;
}

size_t AndConcept::hash() const {
    return dlplan::utils::hash_combine(m_is_static, m_concept_left, m_concept_right);
}

ConceptDenotation AndConcept::evaluate(const State& state) const {
    ConceptDenotation result(state.get_instance_info()->get_objects().size());
    compute_result(
        m_concept_left->evaluate(state),
        m_concept_right->evaluate(state),
        result);
    return result;
}

int AndConcept::compute_complexity() const {
    return m_concept_left->compute_complexity() + m_concept_right->compute_complexity() + 1;
}

void AndConcept::compute_repr(std::stringstream& out) const {
    out << "c_and" << "(";
    m_concept_left->compute_repr(out);
    out << ",";
    m_concept_right->compute_repr(out);
    out << ")";
}

int AndConcept::compute_evaluate_time_score() const {
    return m_concept_left->compute_evaluate_time_score() + m_concept_right->compute_evaluate_time_score() + SCORE_LINEAR;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::AndConcept& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Concept>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::AndConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_concept_left;
    ar << t->m_concept_right;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::AndConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Concept> concept_left;
    std::shared_ptr<const dlplan::core::Concept> concept_right;
    ar >> vocabulary;
    ar >> index;
    ar >> concept_left;
    ar >> concept_right;
    ::new(t)dlplan::core::AndConcept(index, vocabulary, concept_left, concept_right);
}


template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>* t, const unsigned int /*version*/) {
    dlplan::core::AndConcept* first = nullptr;
    std::weak_ptr<dlplan::core::AndConcept>* second = nullptr;
    ar >> const_cast<dlplan::core::AndConcept&>(*first);
    ar >> second;
    ::new(t)std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>(*first, *second);
    delete first;
    delete second;
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::AndConcept& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::AndConcept& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::core::AndConcept* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::core::AndConcept* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>* t, const unsigned int version);
}

BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::core::AndConcept)


namespace std {
    bool less<std::shared_ptr<const dlplan::core::AndConcept>>::operator()(
        const std::shared_ptr<const dlplan::core::AndConcept>& left_concept,
        const std::shared_ptr<const dlplan::core::AndConcept>& right_concept) const {
        return *left_concept < *right_concept;
    }

    std::size_t hash<dlplan::core::AndConcept>::operator()(const dlplan::core::AndConcept& concept_) const {
        return concept_.hash();
    }
}

