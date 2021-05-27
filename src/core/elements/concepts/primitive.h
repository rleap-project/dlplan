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
    PrimitiveConceptElement(const InstanceInfoImpl& parent, const std::string& name, unsigned object_idx)
    : ConceptElement(parent, name), m_object_idx(object_idx) {
        if (!parent.exists_predicate_name(m_name)) {
            throw std::runtime_error("PrimitiveConceptElement::PrimitiveConceptElement - predicate ("s + m_name + ") is missing in InstanceInfo.");
        }
        m_predicate_idx = parent.predicate_idx(m_name);
        unsigned predicate_arity = parent.predicate_arity(m_predicate_idx);
        if (object_idx > predicate_arity) {
            throw std::runtime_error("PrimitiveConceptElement::PrimitiveConceptElement - object index does not match predicate arity ("s + std::to_string(object_idx) + " > " + std::to_string(predicate_arity) + ").");
        }
    }

    virtual unsigned complexity() const override {
        return 1;
    }


    virtual std::string repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << std::to_string(m_object_idx) << ")";
        return ss.str();
    }
};

}
}
}

#endif
