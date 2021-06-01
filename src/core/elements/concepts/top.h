#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_TOP_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_TOP_H_

#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class TopConcept : public Concept {
protected:
    virtual const ConceptDenotation& evaluate_impl(const StateImpl& state) override {
        if (m_result.empty()) {
            unsigned num_objects = state.m_parent->get_num_objects();
            m_result.reserve(num_objects);
            for (unsigned object_idx = 0; object_idx < num_objects; ++object_idx) {
                m_result.push_back(object_idx);
            }
        }
        return m_result;
    }

public:
    TopConcept(const VocabularyInfoImpl& vocabulary)
    : Concept(vocabulary, "c_top") {
    }

    virtual unsigned compute_complexity() const override {
        return 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name;
        return ss.str();
    }
};

}
}
}

#endif
