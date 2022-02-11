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
    }

    ConceptDenotation evaluate(const State& state, PerElementEvaluationCache&, ConceptDenotation result) const override {
        result.get_data().reset();
        result.get_data().set(state.get_instance_info()->get_object_idx(m_constant.get_name()));
        return result;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name << "(" << m_constant.get_name() << ")";
    }
};

}

#endif
