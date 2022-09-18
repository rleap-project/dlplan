#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_NOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_NOT_H_

#include "../concept.h"


namespace dlplan::core::element {

class NotConcept : public Concept {
private:
    void compute_result(const ConceptDenotation& denot, ConceptDenotation& result) const {
        result = denot;
        ~result;
    }

protected:
    const Concept_Ptr m_concept;

public:
    NotConcept(const VocabularyInfo& vocabulary, Concept_Ptr concept)
    : Concept(vocabulary), m_concept(concept){
        if (!concept) {
            throw std::runtime_error("NotConcept::NotConcept - child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_num_objects());
        compute_result(
            m_concept->evaluate(state),
            denotation);
        return denotation;
    }

    const ConceptDenotation* evaluate(const State& state, GeneratorEvaluationCaches& cache) const override {
        auto concept_cache_entry = cache.m_concept_denotation_cache.find(state, *this);
        auto& status = concept_cache_entry->m_status;
        auto& denotation = concept_cache_entry->m_denotation;
        if (status) {
            return &denotation;
        }
        compute_result(
            *m_concept->evaluate(state, cache),
            denotation);
        status = true;
        return &denotation;
    }

    int compute_complexity() const override {
        return m_concept->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_concept->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "c_not";
    }
};

}

#endif
