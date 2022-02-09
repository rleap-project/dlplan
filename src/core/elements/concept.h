#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPT_H_

#include "element.h"


namespace dlplan::core::element {

class Concept : public Element<ConceptDenotation> {
public:
    Concept(const VocabularyInfo& vocabulary, const std::string& name)
    : Element<ConceptDenotation>(vocabulary, name) { }
    ~Concept() override = default;

    virtual ConceptDenotation evaluate(const State& state, EvaluationCaches& caches, ConceptDenotation out) const = 0;

    ConceptDenotation evaluate(const State& state, EvaluationCaches& caches) const override {
        auto cache_result = caches.get_concept_denotation(*this);
        ConceptDenotation result = cache_result.first;
        bool cache_status = cache_result.second;
        if (cache_status) {
            return result;
        } else {
            return evaluate(state, caches, result);
        }
    }
};

}

#endif
