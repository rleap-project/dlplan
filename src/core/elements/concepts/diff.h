#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_DIFF_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_DIFF_H_

#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class DiffConcept : public Concept {
protected:
    const Concept_Ptr m_concept_left;
    const Concept_Ptr m_concept_right;

protected:
    const ConceptDenotation& evaluate_impl(const State& state) override {
        const ConceptDenotation& l_vec = m_concept_left->evaluate(state);
        const ConceptDenotation& r_vec = m_concept_right->evaluate(state);
        ConceptDenotation_Set r_set(l_vec.begin(), l_vec.end());
        for (int c : r_vec) {
            r_set.erase(c);
        }
        m_result.clear();
        m_result.insert(m_result.begin(), r_set.begin(), r_set.end());
        return m_result;
    }

public:
    DiffConcept(const VocabularyInfo& vocabulary, Concept_Ptr concept_1, Concept_Ptr concept_2)
    : Concept(vocabulary, "c_diff"), m_concept_left(concept_1), m_concept_right(concept_2) {
        if (!(concept_1 && concept_2)) {
            throw std::runtime_error("DiffConcept::DiffConcept - at least one child is a nullptr.");
        }
    }

    int compute_complexity() const override {
        return m_concept_left->compute_complexity() + m_concept_right->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_concept_left->compute_repr() << "," << m_concept_right->compute_repr() << ")";
        return ss.str();
    }
};

}
}
}

#endif
