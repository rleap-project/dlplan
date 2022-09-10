#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_AND_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_AND_H_

#include "../concept.h"


namespace dlplan::core::element {

class AndConcept : public Concept {
protected:
    Concept_Ptr m_concept_left;
    Concept_Ptr m_concept_right;

public:
    AndConcept(const VocabularyInfo& vocabulary, Concept_Ptr concept_1, Concept_Ptr concept_2)
    : Concept(vocabulary),
      m_concept_left(concept_1),
      m_concept_right(concept_2) {
        if (!(concept_1 && concept_2)) {
            throw std::runtime_error("AndConcept::AndConcept - at least one child is a nullptr.");
        }
        std::stringstream ss1;
        m_concept_left->compute_repr(ss1);
        std::stringstream ss2;
        m_concept_right->compute_repr(ss2);
        if (ss1.str() > ss2.str()) swap(m_concept_left, m_concept_right);
    }

    ConceptDenotation evaluate(const State& state) const override {
        return m_concept_left->evaluate(state) &= m_concept_right->evaluate(state);
    }

    int compute_complexity() const override {
        return m_concept_left->compute_complexity() + m_concept_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_concept_left->compute_repr(out);
        out << ",";
        m_concept_right->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "c_and";
    }
};

}

#endif
