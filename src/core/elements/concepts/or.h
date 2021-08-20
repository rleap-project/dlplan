#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_OR_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_OR_H_

#include "../concept.h"


namespace dlplan::core::element {

class OrConcept : public Concept {
protected:
    const Concept_Ptr m_concept_left;
    const Concept_Ptr m_concept_right;

public:
    OrConcept(const VocabularyInfo& vocabulary, Concept_Ptr concept_1, Concept_Ptr concept_2)
    : Concept(vocabulary, "c_or"),
      m_concept_left(concept_1->compute_repr() < concept_2->compute_repr() ? concept_1 : concept_2),
      m_concept_right(concept_1->compute_repr() < concept_2->compute_repr() ? concept_2 : concept_1) {
        if (!(concept_1 && concept_2)) {
            throw std::runtime_error("OrConcept::OrConcept - at least one child is a nullptr.");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        const ConceptDenotation l_vec = m_concept_left->evaluate(state);
        const ConceptDenotation r_vec = m_concept_right->evaluate(state);
        ConceptDenotation_Set r_set(l_vec.begin(), l_vec.end());
        r_set.insert(r_vec.begin(), r_vec.end());
        return ConceptDenotation(r_set.begin(), r_set.end());
    }

    int compute_complexity() const override {
        return m_concept_left->compute_complexity() + m_concept_right->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_concept_left->compute_repr() << "," << m_concept_right->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
