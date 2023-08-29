#include "primitive.h"

#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/serialization.hpp>


namespace dlplan::core {
void PrimitiveConcept::compute_result(const State& state, ConceptDenotation& result) const {
    const auto& instance_info = *state.get_instance_info();
    const auto& atoms = instance_info.get_atoms();
    for (int atom_idx : state.get_atom_indices()) {
        const auto& atom = atoms[atom_idx];
        if (atom.get_predicate_index() == m_predicate.get_index()) {
            assert(dlplan::utils::in_bounds(m_pos, atom.get_object_indices()));
            result.insert(atom.get_object_indices()[m_pos]);
        }
    }
    for (const auto &atom : state.get_instance_info()->get_static_atoms()) {
        if (atom.get_predicate_index() == m_predicate.get_index()) {
            assert(dlplan::utils::in_bounds(m_pos, atom.get_object_indices()));
            result.insert(atom.get_object_indices()[m_pos]);
        }
    }
}

ConceptDenotation PrimitiveConcept::evaluate_impl(const State& state, DenotationsCaches&) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        state,
        denotation);
    return denotation;
}

ConceptDenotations PrimitiveConcept::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    ConceptDenotations denotations;
    denotations.reserve(states.size());
    for (size_t i = 0; i < states.size(); ++i) {
        ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            states[i],
            denotation);
        denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
    }
    return denotations;
}

PrimitiveConcept::PrimitiveConcept(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, const Predicate& predicate, int pos)
: Concept(vocabulary_info, index, predicate.is_static()), m_predicate(predicate), m_pos(pos) {
    if (m_pos >= m_predicate.get_arity()) {
        throw std::runtime_error("PrimitiveConcept::PrimitiveConcept - object index does not match predicate arity ("s + std::to_string(m_pos) + " > " + std::to_string(predicate.get_arity()) + ").");
    }
}

ConceptDenotation PrimitiveConcept::evaluate(const State& state) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(state, denotation);
    return denotation;
}

int PrimitiveConcept::compute_complexity() const {
    return 1;
}

void PrimitiveConcept::compute_repr(std::stringstream& out) const {
    out << get_name() << "(" << m_predicate.get_name() << "," << std::to_string(m_pos) << ")";
}

int PrimitiveConcept::compute_evaluate_time_score() const {
    return SCORE_LINEAR;
}

std::string PrimitiveConcept::get_name() {
    return "c_primitive";
}


}

namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::PrimitiveConcept& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Concept>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::PrimitiveConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << &t->m_predicate;
    ar << t->m_pos;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::PrimitiveConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    dlplan::core::Predicate* predicate;
    int pos;
    ar >> vocabulary;
    ar >> index;
    ar >> predicate;
    ar >> pos;
    ::new(t)dlplan::core::PrimitiveConcept(vocabulary, index, *predicate, pos);
    delete predicate;
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::PrimitiveConcept& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::PrimitiveConcept& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::core::PrimitiveConcept* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::core::PrimitiveConcept* t, const unsigned int version);
}

BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::core::PrimitiveConcept)
