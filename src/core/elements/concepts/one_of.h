#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_

#include "../concept.h"


namespace dlp::core::element {

class OneOfConcept : public Concept {
protected:
    const Constant m_constant;

protected:
    const ConceptDenotation& evaluate_impl(const State& state) override {
        if (state.get_instance_info()->get_object(m_constant.get_index()).get_name() != m_constant.get_name()) {
            throw std::runtime_error("OneOfConcept::evaluate_impl - constant does not agree with object of instance.");
        }
        return m_result;
    }

public:
    OneOfConcept(const VocabularyInfo& vocabulary, const Constant& constant)
    : Concept(vocabulary, "c_one_of"), m_constant(constant) {
        m_result = { constant.get_index() };
    }

    int compute_complexity() const override {
        return 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_constant.get_name() << ")";
        return ss.str();
    }
};

}

#endif
