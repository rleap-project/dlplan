#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_NOT_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_NOT_H_

#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class NotConcept : public Concept {
protected:
    Concept_Ptr m_concept;
    ConceptDenotation_Set m_universe_set;

protected:
    virtual const ConceptDenotation& evaluate_impl(const StateImpl& state) override {
        m_result.clear();
        if (m_universe_set.empty()) {
            unsigned num_objects = state.m_parent->get_num_objects();
            for (unsigned object_idx = 0; object_idx < num_objects; ++object_idx) {
                m_universe_set.insert(object_idx);
            }
        }
        ConceptDenotation_Set r_set = m_universe_set;
        const ConceptDenotation& c_vec = m_concept->evaluate(state);
        for (int c : c_vec) {
            r_set.erase(c);
        }
        m_result.insert(m_result.begin(), r_set.begin(), r_set.end());
        return m_result;
    }

public:
    NotConcept(const VocabularyInfoImpl& vocabulary, Concept_Ptr concept)
    : Concept(vocabulary, "c_not"), m_concept(concept) {
        if (!concept) {
            throw std::runtime_error("NotConcept::NotConcept - child is a nullptr");
        }
    }

    virtual unsigned compute_complexity() const override {
        return m_concept->compute_complexity() + 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_concept->compute_repr() << ")";
        return ss.str();
    }
};

}
}
}

#endif
