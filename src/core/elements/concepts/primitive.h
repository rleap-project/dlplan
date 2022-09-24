#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_

#include <sstream>

#include "../concept.h"

namespace dlplan::core::element {

static void collect_concepts(
    const phmap::flat_hash_map<int, std::vector<int>>& per_predicate_idx_atom_idxs,
    const std::vector<Atom>& atoms,
    const Predicate& predicate,
    int pos,
    ConceptDenotation& result) {
    auto it = per_predicate_idx_atom_idxs.find(predicate.get_index());
    if (it != per_predicate_idx_atom_idxs.end()) {
        for (int atom_idx : it->second) {
            const auto& atom = atoms[atom_idx];
            result.insert(atom.get_object_ref(pos).get_index());
        }
    }
}

class PrimitiveConcept : public Concept {
private:
    void compute_result(const State& state, ConceptDenotation& result) const {
        const auto& atoms = state.get_instance_info_ref().get_atoms_ref();
        const auto& static_atoms = state.get_instance_info_ref().get_static_atoms_ref();
        collect_concepts(state.get_per_predicate_idx_atom_idxs_ref(), atoms, m_predicate, m_pos, result);
        collect_concepts(state.get_instance_info_ref().get_per_predicate_idx_static_atom_idxs_ref(), static_atoms, m_predicate, m_pos, result);

    }

    std::unique_ptr<ConceptDenotation> evaluate_impl(const State& state, DenotationsCaches&) const override {
        auto denotation = std::make_unique<ConceptDenotation>(
            ConceptDenotation(state.get_instance_info_ref().get_num_objects()));
        compute_result(
            state,
            *denotation);
        return denotation;
    }

    std::unique_ptr<ConceptDenotations> evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        auto denotations = std::make_unique<ConceptDenotations>();
        denotations->reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            auto denotation = std::make_unique<ConceptDenotation>(
                ConceptDenotation(states[i].get_instance_info_ref().get_num_objects()));
            compute_result(
                states[i],
                *denotation);
            denotations->push_back(caches.m_c_denot_cache.insert(std::move(denotation)).first->get());
        }
        return denotations;
    }

protected:
    const Predicate m_predicate;
    const int m_pos;

public:
    PrimitiveConcept(const VocabularyInfo& vocabulary, const Predicate& predicate, int pos)
    : Concept(vocabulary), m_predicate(predicate), m_pos(pos) {
        if (m_pos >= m_predicate.get_arity()) {
            throw std::runtime_error("PrimitiveConcept::PrimitiveConcept - object index does not match predicate arity ("s + std::to_string(m_pos) + " > " + std::to_string(predicate.get_arity()) + ").");
        }
        if (!vocabulary.exists_predicate(m_predicate)) {
            throw std::runtime_error("PrimitiveConcept::PrimitiveConcept - predicate does not exist in VocabularyInfo.");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        ConceptDenotation denotation(state.get_instance_info_ref().get_num_objects());
        compute_result(state, denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return 1;
    }


    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(" << m_predicate.get_name_ref() << "," << std::to_string(m_pos) << ")";
    }

    static std::string get_name() {
        return "c_primitive";
    }
};

}

#endif
