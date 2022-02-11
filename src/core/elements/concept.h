#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPT_H_

#include "element.h"


namespace dlplan::core::element {

class Concept : public Element<ConceptDenotation> {
public:
    Concept(const VocabularyInfo& vocabulary, const std::string& name)
    : Element<ConceptDenotation>(vocabulary, name) { }
    ~Concept() override = default;

    virtual ConceptDenotation evaluate(const State& state, PerElementEvaluationCache& caches, ConceptDenotation out) const = 0;

    ConceptDenotation evaluate(const State& state, PerElementEvaluationCache& caches) const override {
        return caches.retrieve_or_evaluate(state, *this);
    }
};

}

#endif
