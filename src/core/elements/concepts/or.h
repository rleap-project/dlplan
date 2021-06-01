#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_OR_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_OR_H_

#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class OrConcept : public Concept {
protected:
    Concept_Ptr m_l;
    Concept_Ptr m_r;

protected:
    virtual const ConceptDenotation& evaluate_impl(const StateImpl& state) override {
        const ConceptDenotation& l_vec = m_l->evaluate(state);
        const ConceptDenotation& r_vec = m_r->evaluate(state);
        ConceptDenotation_Set r_set(l_vec.begin(), l_vec.end());
        r_set.insert(r_vec.begin(), r_vec.end());
        m_result.clear();
        m_result.insert(m_result.begin(), r_set.begin(), r_set.end());
        return m_result;
    }

public:
    OrConcept(const VocabularyInfoImpl& vocabulary, Concept_Ptr l, Concept_Ptr r)
    : Concept(vocabulary, "c_and") {
        if (!(l && r)) {
            throw std::runtime_error("OrConcept::make_concept_element - children are not of type Concept.");
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
