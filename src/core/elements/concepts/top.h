#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_TOP_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_TOP_H_

#include "../concept.h"


namespace dlplan::core::element {

class TopConcept : public Concept {
private:
    mutable ConceptDenotation m_result;

    mutable int m_num_objects;

public:
    TopConcept(const VocabularyInfo& vocabulary)
    : Concept(vocabulary, "c_top"), m_num_objects(-1) {
    }

    ConceptDenotation evaluate(const State& state) const override {
        int num_objects = state.get_instance_info()->get_num_objects();
        if (m_num_objects != num_objects) {
            m_result.clear();
            m_result.reserve(num_objects);
            for (int object_idx = 0; object_idx < num_objects; ++object_idx) {
                m_result.push_back(object_idx);
            }
            m_num_objects = num_objects;
        }
        return m_result;
    }

    int compute_complexity() const override {
        return 1;
    }

    std::string compute_repr() const override {
        return m_name;
    }
};

}

#endif
