#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_

#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class PrimitiveConceptElement : public ConceptElement {
protected:
    unsigned m_predicate_idx;
    unsigned m_pos;

protected:
    virtual Concepts evaluate_impl(const StateImpl& state) override {
        m_result.clear();
        for (unsigned atom_idx : state.m_atoms) {
            const AtomImpl& atom = state.m_parent->atom(atom_idx);
            if (atom.predicate_idx() == m_predicate_idx) {
                m_result.push_back(atom.object_idx(m_pos));
            }
        }
        return m_result;
    }

public:
    PrimitiveConceptElement(std::shared_ptr<InstanceInfoImpl> parent, const std::string& name, unsigned pos)
    : ConceptElement(parent, name), m_pos(pos) {
        if (!parent->exists_predicate_name(name)) {
            throw std::runtime_error("PrimitiveConceptElement::PrimitiveConceptElement - predicate ("s + m_name + ") is missing in InstanceInfo.");
        }
        m_predicate_idx = parent->predicate_idx(name);
        unsigned predicate_arity = parent->predicate_arity(m_predicate_idx);
        if (pos >= predicate_arity) {
            throw std::runtime_error("PrimitiveConceptElement::PrimitiveConceptElement - object index does not match predicate arity ("s + std::to_string(pos) + " > " + std::to_string(predicate_arity) + ").");
        }
    }

    virtual unsigned complexity() const override {
        return 1;
    }


    virtual std::string repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << std::to_string(m_pos) << ")";
        return ss.str();
    }
};

}
}
}

#endif
