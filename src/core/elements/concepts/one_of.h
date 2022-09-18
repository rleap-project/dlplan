#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_

#include "../concept.h"


namespace dlplan::core::element {

class OneOfConcept : public Concept {
protected:
    const Constant m_constant;

public:
    OneOfConcept(const VocabularyInfo& vocabulary, const Constant& constant)
    : Concept(vocabulary), m_constant(constant) {
    }

    ConceptDenotation evaluate(const State& state) const override {
        if (!state.get_instance_info()->exists_object(m_constant.get_name())) {
            throw std::runtime_error("OneOfConcept::evaluate - no object with name of constant exists in instance: (" + m_constant.get_name() + ")");
        }
        ConceptDenotation result(state.get_instance_info()->get_num_objects());
        result.insert(state.get_instance_info()->get_object_idx(m_constant.get_name()));
        return result;
    }

    const ConceptDenotation* evaluate(const State& state, GeneratorEvaluationCaches& cache) const override {
        auto concept_cache_entry = cache.m_concept_denotation_cache.find(state, *this);
        auto& status = concept_cache_entry->m_status;
        auto& denotation = concept_cache_entry->m_denotation;
        if (status) {
            return &denotation;
        }
        denotation = evaluate(state);
        status = true;
        return &denotation;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(" << m_constant.get_name() << ")";
    }

    static std::string get_name() {
        return "c_one_of";
    }
};

}

#endif
