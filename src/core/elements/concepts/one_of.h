#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_

#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class OneOfConcept : public Concept {
protected:
    std::string m_object_name;

protected:
    virtual const ConceptDenotation& evaluate_impl(const StateImpl& state) override {
        unsigned object_idx = state.m_parent->get_object_idx(m_object_name);
        m_result = { static_cast<int>(object_idx) };
        return m_result;
    }

public:
    OneOfConcept(const VocabularyInfoImpl& vocabulary, std::string object_name)
    : Concept(vocabulary, "c_one_of"), m_object_name(object_name) {
    }

    virtual unsigned compute_complexity() const override {
        return 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_object_name << ")";
        return ss.str();
    }
};

}
}
}

#endif
