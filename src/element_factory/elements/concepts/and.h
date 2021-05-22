#ifndef DLP_SRC_ELEMENT_FACTORY_ELEMENTS_CONCEPTS_AND_H_
#define DLP_SRC_ELEMENT_FACTORY_ELEMENTS_CONCEPTS_AND_H_

#include "../concept.h"
#include "../../types.h"

namespace dlp {

class AndConceptElement : public ConceptElement {
protected:
    ConceptElement_Ptr m_l;
    ConceptElement_Ptr m_r;

protected:
    virtual Concepts evaluate_impl(const Index_Vec& atoms) override {
        // TODO(dominik):
    }

public:
    AndConceptElement(bool goal, const Index_Vec& goal_atoms, ConceptElement_Ptr l, ConceptElement_Ptr r)
    : ConceptElement(goal, goal_atoms), m_l(l), m_r(r) {}
};

}

#endif
