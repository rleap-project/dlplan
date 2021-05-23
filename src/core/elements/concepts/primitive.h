#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_

#include "../concept.h"
#include "../../types.h"

namespace dlp {
namespace core {

class PrimitiveConceptElement : public ConceptElement {
protected:
    unsigned m_predicate_idx;
    unsigned m_object_idx;

protected:
    virtual Concepts evaluate_impl(const Index_Vec& atoms) override {
        // TODO(dominik):
    }

public:
    PrimitiveConceptElement(std::shared_ptr<TaskInfo> task_info, bool goal, unsigned predicate_idx, unsigned object_idx)
    : ConceptElement(task_info, goal), m_predicate_idx(predicate_idx), m_object_idx(object_idx) {}
};

}
}

#endif
