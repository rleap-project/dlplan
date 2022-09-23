#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPT_H_

#include "element.h"


namespace dlplan::core::element {

class Concept : public Element<ConceptDenotation> {
public:
    explicit Concept(const VocabularyInfo& vocabulary) : Element<ConceptDenotation>(vocabulary) { }
    ~Concept() override = default;

    virtual ConceptDenotation evaluate(const State& state) const = 0;
    virtual ConceptDenotation* evaluate(const State& state, DenotationsCaches& caches) const = 0;
    virtual ConceptDenotations* evaluate(const States& states, DenotationsCaches& caches) const = 0;
};

}

#endif
