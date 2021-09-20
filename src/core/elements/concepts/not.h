#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_NOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_NOT_H_

#include "../concept.h"


namespace dlplan::core::element {

class NotConcept : public Concept {
protected:
    const Concept_Ptr m_concept;
    mutable ConceptDenotation_Set m_universe_set;

    mutable int m_num_objects;

public:
    NotConcept(const VocabularyInfo& vocabulary, Concept_Ptr concept)
    : Concept(vocabulary, "c_not"), m_concept(concept) {
        if (!concept) {
            throw std::runtime_error("NotConcept::NotConcept - child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        int num_objects = state.get_instance_info()->get_num_objects();
        if (m_num_objects != num_objects) {
            m_universe_set.clear();
            for (int object_idx = 0; object_idx < num_objects; ++object_idx) {
                m_universe_set.insert(object_idx);
            }
            m_num_objects = num_objects;
        }
        ConceptDenotation_Set r_set = m_universe_set;
        const ConceptDenotation c_vec = m_concept->evaluate(state);
        for (int c : c_vec) {
            r_set.erase(c);
        }
        return ConceptDenotation(r_set.begin(), r_set.end());
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
