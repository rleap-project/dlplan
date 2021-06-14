#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_TOP_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_TOP_H_

#include "../concept.h"


namespace dlplan::core::element {

class TopConcept : public Concept {
protected:
    const ConceptDenotation& evaluate_impl(const State& state) override {
        if (m_result.empty()) {
            int num_objects = state.get_instance_info()->get_num_objects();
            m_result.reserve(num_objects);
            for (int object_idx = 0; object_idx < num_objects; ++object_idx) {
                m_result.push_back(object_idx);
            }
        }
        return m_result;
    }

public:
    TopConcept(const VocabularyInfo& vocabulary)
    : Concept(vocabulary, "c_top") {
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
