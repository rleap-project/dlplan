#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_NOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_NOT_H_

#include "../concept.h"


namespace dlplan::core::element {

class NotConcept : public Concept {
protected:
    const Concept_Ptr m_concept;

public:
    NotConcept(const VocabularyInfo& vocabulary, Concept_Ptr concept)
    : Concept(vocabulary, "c_not"), m_concept(concept){
        if (!concept) {
            throw std::runtime_error("NotConcept::NotConcept - child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state, PerElementEvaluationCache& caches, ConceptDenotation result) const override {
        ConceptDenotation c = m_concept->evaluate(state, caches);
        dlplan::utils::BitsetView result_data = result.get_data();
        result_data.set(c.get_data());
        ~result_data;
        return result;
    }

    int compute_complexity() const override {
        return m_concept->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name << "(";
        m_concept->compute_repr(out);
        out << ")";
    }
};

}

#endif
