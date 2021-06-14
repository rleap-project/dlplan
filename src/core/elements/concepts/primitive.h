#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_

#include <algorithm>

#include "../concept.h"

namespace dlplan::core::element {

class PrimitiveConcept : public Concept {
protected:
    const Predicate m_predicate;
    const int m_pos;

protected:
    const ConceptDenotation& evaluate_impl(const State& state) override {
        const InstanceInfo& info = *state.get_instance_info();
        ConceptDenotation_Set result_set;
        for (int atom_idx : state.get_atom_idxs()) {
            const Atom& atom = info.get_atom(atom_idx);
            if (atom.get_predicate().get_index() == m_predicate.get_index()) {
                result_set.insert(atom.get_object(m_pos).get_index());
            }
        }
        m_result.clear();
        m_result.insert(m_result.end(), result_set.begin(), result_set.end());
        return m_result;
    }

public:
    PrimitiveConcept(const VocabularyInfo& vocabulary, const Predicate& predicate, int pos)
    : Concept(vocabulary, predicate.get_name()), m_predicate(predicate), m_pos(pos) {
        if (m_pos >= predicate.get_arity()) {
            throw std::runtime_error("PrimitiveConcept::PrimitiveConcept - object index does not match predicate arity ("s + std::to_string(m_pos) + " > " + std::to_string(predicate.get_arity()) + ").");
        }
        if (predicate.get_vocabulary_info() != &vocabulary) {
            throw std::runtime_error("PrimitiveConcept::PrimitiveConcept - predicate does not come from same vocabulary.");
        }
    }

    int compute_complexity() const override {
        return 1;
    }


    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << std::to_string(m_pos) << ")";
        return ss.str();
    }
};

}

#endif
