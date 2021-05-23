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
    AndConceptElement(std::shared_ptr<TaskInfo> task_info, bool goal, ConceptElement_Ptr l, ConceptElement_Ptr r)
    : ConceptElement(task_info, goal), m_l(l), m_r(r) {}
};

}

#endif
