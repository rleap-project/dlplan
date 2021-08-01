#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_

#include "../concept.h"


namespace dlplan::core::element {

class OneOfConcept : public Concept {
protected:
    const Constant m_constant;

public:
    OneOfConcept(const VocabularyInfo& vocabulary, const Constant& constant)
    : Concept(vocabulary, "c_one_of"), m_constant(constant) {
        m_result = { constant.get_index() };
    }

    const ConceptDenotation& evaluate(const State& state) override {
        if (state.get_instance_info()->get_object(m_constant.get_index()).get_name() != m_constant.get_name()) {
            throw std::runtime_error("OneOfConcept::evaluate - constant does not agree with object of instance.");
        }
        return m_result;
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
