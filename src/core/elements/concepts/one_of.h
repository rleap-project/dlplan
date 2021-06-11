#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_

#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class OneOfConcept : public Concept {
protected:
    const std::string m_object_name;

protected:
    const ConceptDenotation& evaluate_impl(const State& state) override {
        int object_idx = state.get_instance_info()->get_object_idx(m_object_name);
        m_result = { object_idx };
        return m_result;
    }

public:
    OneOfConcept(const VocabularyInfo& vocabulary, const std::string& object_name)
    : Concept(vocabulary, "c_one_of"), m_object_name(object_name) {
    }

    int compute_complexity() const override {
        return 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_object_name << ")";
        return ss.str();
    }
};

}
}
}

#endif
