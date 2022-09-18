#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_DIFF_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_DIFF_H_

#include "../concept.h"


namespace dlplan::core::element {

class DiffConcept : public Concept {
private:
    void compute_result(const ConceptDenotation& left_denot, const ConceptDenotation& right_denot, ConceptDenotation& result) const {
        result = left_denot;
        result -= right_denot;
    }

protected:
    const Concept_Ptr m_concept_left;
    const Concept_Ptr m_concept_right;

public:
    DiffConcept(const VocabularyInfo& vocabulary, Concept_Ptr concept_1, Concept_Ptr concept_2)
    : Concept(vocabulary), m_concept_left(concept_1), m_concept_right(concept_2) {
        if (!(concept_1 && concept_2)) {
            throw std::runtime_error("DiffConcept::DiffConcept - at least one child is a nullptr.");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        ConceptDenotation result(state.get_instance_info()->get_num_objects());
        compute_result(
            m_concept_left->evaluate(state),
            m_concept_right->evaluate(state),
            result);
        return result;
    }

    const ConceptDenotation* evaluate(const State& state, GeneratorEvaluationCaches& cache) const override {
        auto concept_cache_entry = cache.m_concept_denotation_cache.find(state, *this);
        auto& status = concept_cache_entry->m_status;
        auto& denotation = concept_cache_entry->m_denotation;
        if (status) {
            return &denotation;
        }
        compute_result(
            *m_concept_left->evaluate(state, cache),
            *m_concept_right->evaluate(state, cache),
            denotation);
        status = true;
        return &denotation;
    }

    int compute_complexity() const override {
        return m_concept_left->compute_complexity() + m_concept_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_concept_left->compute_repr(out);
        out << ",";
        m_concept_right->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "c_diff";
    }
};

}

#endif
