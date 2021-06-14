#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_

#include "../concept.h"


namespace dlplan::core::element {

class BotConcept : public Concept {
protected:
    const ConceptDenotation& evaluate_impl(const State&) override {
        return m_result;
    }

public:
    BotConcept(const VocabularyInfo& vocabulary)
    : Concept(vocabulary, "c_bot") {
    }

    int compute_complexity() const override {
        return 1;
    }

    std::string compute_repr() const override {
        return m_name;
    }
};

}

#endif
