#include "../../../../include/dlplan/core/elements/concepts/primitive.h"

#include "../../../utils/collections.h"


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
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

PrimitiveConcept::PrimitiveConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Predicate& predicate, int pos)
    : Concept(index, vocabulary_info, predicate.is_static()), m_predicate(predicate), m_pos(pos) {
    if (m_pos >= m_predicate.get_arity()) {
        throw std::runtime_error("PrimitiveConcept::PrimitiveConcept - object index does not match predicate arity ("s + std::to_string(m_pos) + " > " + std::to_string(predicate.get_arity()) + ").");
    }
}

bool PrimitiveConcept::are_equal_impl(const Concept& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const PrimitiveConcept&>(other);
        return m_is_static == other_derived.m_is_static
            && m_predicate == other_derived.m_predicate
            && m_pos == other_derived.m_pos;
    }
    return false;
}

size_t PrimitiveConcept::hash_impl() const {
    return hash_combine(m_is_static, m_predicate, m_pos);
}

ConceptDenotation PrimitiveConcept::evaluate(const State& state) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(state, denotation);
    return denotation;
}

int PrimitiveConcept::compute_complexity_impl() const {
    return 1;
}

void PrimitiveConcept::str_impl(std::stringstream& out) const {
    out << "c_primitive" << "(" << m_predicate.get_name() << "," << std::to_string(m_pos) << ")";
}

int PrimitiveConcept::compute_evaluate_time_score_impl() const {
    return SCORE_LINEAR;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::PrimitiveConcept>>::operator()(
        const std::shared_ptr<const dlplan::core::PrimitiveConcept>& left_concept,
        const std::shared_ptr<const dlplan::core::PrimitiveConcept>& right_concept) const {
        return *left_concept < *right_concept;
    }

    std::size_t hash<dlplan::core::PrimitiveConcept>::operator()(const dlplan::core::PrimitiveConcept& concept_) const {
        return concept_.hash();
    }
}
