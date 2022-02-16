#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPT_H_

#include "element.h"


namespace dlplan::core::element {

class Concept : public Element<ConceptDenotation> {
public:
    Concept(const VocabularyInfo& vocabulary, const std::string& name) : Element<ConceptDenotation>(vocabulary, name) { }
    ~Concept() override = default;

    ConceptDenotation evaluate(PerElementEvaluationContext& context) const override {
        return context.cache.retrieve_or_evaluate(*this, context);
    }

    virtual void evaluate(PerElementEvaluationContext& context, ConceptDenotation& result) const = 0;
};

}

#endif
