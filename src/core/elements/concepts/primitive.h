#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_

#include "../utils.h"

#include "../../../../include/dlplan/core.h"

#include "../../../utils/collections.h"

#include <sstream>

using namespace std::string_literals;


namespace dlplan::core {

class PrimitiveConcept : public Concept {
private:
    void compute_result(const State& state, ConceptDenotation& result) const {
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

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches&) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            state,
            denotation);
        return denotation;
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                states[i],
                denotation);
            denotations.push_back(caches.get_concept_denotation_cache().insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

protected:
    const Predicate m_predicate;
    const int m_pos;

public:
    PrimitiveConcept(std::shared_ptr<const VocabularyInfo> vocabulary_info, const Predicate& predicate, int pos)
    : Concept(vocabulary_info, predicate.is_static()), m_predicate(predicate), m_pos(pos) {
        if (m_pos >= m_predicate.get_arity()) {
            throw std::runtime_error("PrimitiveConcept::PrimitiveConcept - object index does not match predicate arity ("s + std::to_string(m_pos) + " > " + std::to_string(predicate.get_arity()) + ").");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(state, denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(" << m_predicate.get_name() << "," << std::to_string(m_pos) << ")";
    }

    int compute_evaluate_time_score() const override {
        return SCORE_LINEAR;
    }

    static std::string get_name() {
        return "c_primitive";
    }
};

}

#endif
