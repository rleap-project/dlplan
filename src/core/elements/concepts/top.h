#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_TOP_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_TOP_H_

#include "../concept.h"


namespace dlplan::core::element {

class TopConcept : public Concept {
public:
    TopConcept(const VocabularyInfo& vocabulary)
    : Concept(vocabulary, "c_top") {
    }

    void evaluate(PerElementEvaluationContext& context, ConceptDenotation& result) const override {
        result.get_data().set();
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name;
    }
};

}

#endif
