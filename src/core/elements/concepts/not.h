#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_NOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_NOT_H_

#include "../concept.h"


namespace dlplan::core::element {

class NotConcept : public Concept {
private:
    ConceptDenotation compute_result(ConceptDenotation&& denot) const {
        return ~denot;
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
        return compute_result(m_concept->evaluate(state));
    }

    ConceptDenotation evaluate(const State& state, EvaluationCaches& cache) const override {
        if (cache.m_concept_denotation_cache.count(state, *this)) {
            return cache.m_concept_denotation_cache.find(state, *this);
        }
        auto result = compute_result(m_concept->evaluate(state, cache));
        cache.m_concept_denotation_cache.insert(state, *this, result);
        return result;
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
