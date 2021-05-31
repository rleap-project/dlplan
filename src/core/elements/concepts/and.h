#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_AND_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_AND_H_

#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class AndConceptElement : public ConceptElement {
protected:
    ConceptElement_Ptr m_l;
    ConceptElement_Ptr m_r;

protected:
    virtual const Concepts& evaluate_impl(const StateImpl& state) override {
        const Concepts& l_vec = m_l->evaluate(state);
        const Concepts& r_vec = m_r->evaluate(state);
        Concepts_Set r_set(r_vec.begin(), r_vec.end());
        m_result.clear();
        for (Concept c : l_vec) {
            if (r_set.find(c) != r_set.end()) {
                m_result.push_back(c);
            }
        }
        return m_result;
    }

public:
    AndConceptElement(ConceptElement_Ptr l, ConceptElement_Ptr r)
    : ConceptElement("c_and") {
        if (!(l && r)) {
            throw std::runtime_error("AndConceptExpression::make_concept_element - children are not of type ConceptElement.");
        }
        // Element is commutative. Hence sort lexicographically.
        if (l->compute_repr() < r->compute_repr()) {
            m_l = l;
            m_r = r;
        } else {
            m_l = r;
            m_r = l;
        }
    }

    virtual unsigned compute_complexity() const override {
        return m_l->compute_complexity() + m_r->compute_complexity() + 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_l->compute_repr() << "," << m_r->compute_repr() << ")";
        return ss.str();
    }
};

}
}
}

#endif
