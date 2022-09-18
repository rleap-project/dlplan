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
            result.insert(atom.get_object(pos).get_index());
        }
    }
}

class PrimitiveConcept : public Concept {
private:
    void compute_result(const State& state, ConceptDenotation& result) const {
        const InstanceInfo& info = *state.get_instance_info();
        const auto& atoms = info.get_atoms();
        const auto& static_atoms = info.get_static_atoms();
        collect_concepts(state.get_per_predicate_idx_atom_idxs(), atoms, m_predicate, m_pos, result);
        collect_concepts(info.get_per_predicate_idx_static_atom_idxs(), static_atoms, m_predicate, m_pos, result);

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
        ConceptDenotation denotation(state.get_instance_info()->get_num_objects());
        compute_result(state, denotation);
        return denotation;
    }

    const ConceptDenotation* evaluate(const State& state, GeneratorEvaluationCaches& cache) const override {
        auto concept_cache_entry = cache.m_concept_denotation_cache.find(state, *this);
        auto& status = concept_cache_entry->m_status;
        auto& denotation = concept_cache_entry->m_denotation;
        if (status) {
            return &denotation;
        }
        compute_result(
            state,
            denotation);
        status = true;
        return &denotation;
    }

    int compute_complexity() const override {
        return 1;
    }


    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(" << m_predicate.get_name() << "," << std::to_string(m_pos) << ")";
    }

    static std::string get_name() {
        return "c_primitive";
    }
};

}

#endif
