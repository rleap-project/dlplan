#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_NOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_NOT_H_

#include "../concept.h"


namespace dlplan::core::element {

class NotConcept : public Concept {
protected:
    const Concept_Ptr m_concept;
    ConceptDenotation_Set m_universe_set;

protected:
    const ConceptDenotation& evaluate_impl(const State& state) override {
        if (m_universe_set.empty()) {
            int num_objects = state.get_instance_info()->get_num_objects();
            for (int object_idx = 0; object_idx < num_objects; ++object_idx) {
                m_universe_set.insert(object_idx);
            }
        }
        ConceptDenotation_Set r_set = m_universe_set;
        const ConceptDenotation& c_vec = m_concept->evaluate(state);
        for (int c : c_vec) {
            r_set.erase(c);
        }
        m_result.clear();
        m_result.insert(m_result.begin(), r_set.begin(), r_set.end());
        return m_result;
    }

public:
    NotConcept(const VocabularyInfo& vocabulary, Concept_Ptr concept)
    : Concept(vocabulary, "c_not"), m_concept(concept) {
        if (!concept) {
            throw std::runtime_error("NotConcept::NotConcept - child is a nullptr");
        }
    }

    int compute_complexity() const override {
        return m_concept->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_concept->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
