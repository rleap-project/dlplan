#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_

#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class PrimitiveConceptElement : public ConceptElement {
protected:
    unsigned m_predicate_idx;
    unsigned m_object_idx;

protected:
    virtual Concepts evaluate_impl(const StateImpl& state) override {
        // TODO(dominik):
    }

public:
    PrimitiveConceptElement(const InstanceInfoImpl* parent, unsigned predicate_idx, unsigned object_idx)
    : ConceptElement(parent), m_predicate_idx(predicate_idx), m_object_idx(object_idx) {}

    virtual unsigned complexity() const override {
        return 1;
    }
};

}
}
}

#endif
