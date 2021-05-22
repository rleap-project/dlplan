#ifndef DLP_SRC_ELEMENT_FACTORY_ELEMENTS_CONCEPT_H_
#define DLP_SRC_ELEMENT_FACTORY_ELEMENTS_CONCEPT_H_

#include "../element.h"
#include "../types.h"

namespace dlp {

class ConceptElement : public Element<Concepts> {
public:
    ConceptElement(bool goal, const Index_Vec& goal_atoms)
    : Element<Concepts>(goal, goal_atoms) { }

    size_t size() const override {
        return m_result.size();
    }
};

}

#endif
