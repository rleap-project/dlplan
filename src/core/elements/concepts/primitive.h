#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_

#include <sstream>

#include "../concept.h"

namespace dlplan::core::element {

class PrimitiveConcept : public Concept {
protected:
    const Predicate m_predicate;
    const int m_pos;

public:
    PrimitiveConcept(const VocabularyInfo& vocabulary, const Predicate& predicate, int pos)
    : Concept(vocabulary, "c_primitive"), m_predicate(predicate), m_pos(pos) {
        if (m_pos >= predicate.get_arity()) {
            throw std::runtime_error("PrimitiveConcept::PrimitiveConcept - object index does not match predicate arity ("s + std::to_string(m_pos) + " > " + std::to_string(predicate.get_arity()) + ").");
        }
        if (!vocabulary.exists_predicate(predicate)) {
            throw std::runtime_error("PrimitiveConcept::PrimitiveConcept - predicate does not exist in VocabularyInfo.");
        }
    }

    void evaluate(PerElementEvaluationContext& context, ConceptDenotation& result) const override {
        const auto& state = *context.state;
        const InstanceInfo& info = *state.get_instance_info();
        int num_objects = info.get_num_objects();
        auto& result_data = result.get_data();
        result_data.reset();
        const auto& atoms = info.get_atoms();
        const auto& per_predicate_idx_static_atom_idxs = state.get_per_predicate_idx_static_atom_idxs();
        auto it = per_predicate_idx_static_atom_idxs.find(m_predicate.get_index());
        if (it != per_predicate_idx_static_atom_idxs.end()) {
            for (int atom_idx : it->second) {
                const auto& atom = atoms[atom_idx];
                result_data.set(atom.get_object(m_pos).get_index());
            }
        }
    }

    int compute_complexity() const override {
        return 1;
    }


    void compute_repr(std::stringstream& out) const override {
        out << m_name << "(" << m_predicate.get_name() << "," << std::to_string(m_pos) << ")";
    }
};

}

#endif
