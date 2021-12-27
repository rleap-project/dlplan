#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_AND_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_AND_H_

#include "../concept.h"


namespace dlplan::core::element {

class AndConcept : public Concept {
protected:
    const Concept_Ptr m_concept_left;
    const Concept_Ptr m_concept_right;

public:
    AndConcept(const VocabularyInfo& vocabulary, Concept_Ptr concept_1, Concept_Ptr concept_2)
    : Concept(vocabulary, "c_and"),
      m_concept_left(concept_1->compute_repr() < concept_2->compute_repr() ? concept_1 : concept_2),
      m_concept_right(concept_1->compute_repr() < concept_2->compute_repr() ? concept_2 : concept_1) {
        if (!(concept_1 && concept_2)) {
            throw std::runtime_error("AndConcept::AndConcept - at least one child is a nullptr.");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        auto l = m_concept_left->evaluate(state);
        const auto r = m_concept_right->evaluate(state);
        l.get_data() &= r.get_const_data();
        return l;
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
