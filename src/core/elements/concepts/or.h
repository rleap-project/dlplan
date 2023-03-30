#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_OR_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_OR_H_

#include "../concept.h"


namespace dlplan::core::element {

class OrConcept : public Concept {
private:
    void compute_result(const ConceptDenotation& left_denot, const ConceptDenotation& right_denot, ConceptDenotation& result) const {
        result = left_denot;
        result |= right_denot;
    }

    std::unique_ptr<ConceptDenotation> evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        auto denotation = std::make_unique<ConceptDenotation>(
            ConceptDenotation(state.get_instance_info()->get_objects().size()));
        denotation->set();
        compute_result(
            *m_concept_left->evaluate(state, caches),
            *m_concept_right->evaluate(state, caches),
            *denotation);
        return denotation;
    }

    std::unique_ptr<ConceptDenotations> evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        auto denotations = std::make_unique<ConceptDenotations>();
        denotations->reserve(states.size());
        auto concept_left_denotations = m_concept_left->evaluate(states, caches);
        auto concept_right_denotations = m_concept_right->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            const auto& state = states[i];
            int num_objects = state.get_instance_info()->get_objects().size();
            auto denotation = std::make_unique<ConceptDenotation>(ConceptDenotation(num_objects));
            compute_result(
                *(*concept_left_denotations)[i],
                *(*concept_right_denotations)[i],
                *denotation);
            denotations->push_back(caches.m_c_denot_cache.insert(std::move(denotation)).first->get());
        }
        return denotations;
    }

protected:
    Concept_Ptr m_concept_left;
    Concept_Ptr m_concept_right;

public:
    OrConcept(const VocabularyInfo& vocabulary, Concept_Ptr concept_1, Concept_Ptr concept_2)
    : Concept(vocabulary, concept_1->is_static() && concept_2->is_static()),
      m_concept_left(concept_1),
      m_concept_right(concept_2) {
        if (!(concept_1 && concept_2)) {
            throw std::runtime_error("OrConcept::OrConcept - at least one child is a nullptr.");
        }
        std::stringstream ss1;
        m_concept_left->compute_repr(ss1);
        std::stringstream ss2;
        m_concept_right->compute_repr(ss2);
        if (ss1.str() > ss2.str()) swap(m_concept_left, m_concept_right);
    }

    ConceptDenotation evaluate(const State& state) const override {
        ConceptDenotation result(state.get_instance_info()->get_objects().size());
        compute_result(
            m_concept_left->evaluate(state),
            m_concept_right->evaluate(state),
            result);
        return result;
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
        return "c_or";
    }
};

}

#endif
