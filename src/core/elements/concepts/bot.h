#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_

#include "../concept.h"


namespace dlplan::core::element {

class BotConcept : public Concept {
public:
    BotConcept(const VocabularyInfo& vocabulary)
    : Concept(vocabulary, "c_bot") { }

    void evaluate(PerElementEvaluationContext& context, ConceptDenotation& result) const override {
        result.get_data().reset();
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
