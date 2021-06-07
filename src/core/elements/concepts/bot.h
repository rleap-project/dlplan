#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_

#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class BotConcept : public Concept {
protected:
    virtual const ConceptDenotation& evaluate_impl(const State& state) override {
        return m_result;
    }

public:
    BotConcept(const VocabularyInfo& vocabulary)
    : Concept(vocabulary, "c_bot") {
    }

    virtual unsigned compute_complexity() const override {
        return 1;
    }

    virtual std::string compute_repr() const override {
        return m_name;
    }
};

}
}
}

#endif
