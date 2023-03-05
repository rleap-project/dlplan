#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_

#include "../concept.h"


namespace dlplan::core::element {

class OneOfConcept : public Concept {
private:
    void compute_result(const State& state, ConceptDenotation& result) const {
        result.insert(state.get_instance_info_ref().get_object_idx(m_constant.get_name_ref()));
    }

    std::unique_ptr<ConceptDenotation> evaluate_impl(const State& state, DenotationsCaches&) const override {
        auto denotation = std::make_unique<ConceptDenotation>(
            ConceptDenotation(state.get_instance_info_ref().get_num_objects()));
        compute_result(
            state,
            *denotation);
        return denotation;
    }

    std::unique_ptr<ConceptDenotations> evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        auto denotations = std::make_unique<ConceptDenotations>();
        denotations->reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            auto denotation = std::make_unique<ConceptDenotation>(
                ConceptDenotation(states[i].get_instance_info_ref().get_num_objects()));
            compute_result(
                states[i],
                *denotation);
            denotations->push_back(caches.m_c_denot_cache.insert(std::move(denotation)).first->get());
        }
        return denotations;
    }

protected:
    const Constant m_constant;

public:
    OneOfConcept(const VocabularyInfo& vocabulary, const Constant& constant)
    : Concept(vocabulary, true), m_constant(constant) {
    }

    ConceptDenotation evaluate(const State& state) const override {
        if (!state.get_instance_info_ref().exists_object(m_constant.get_name_ref())) {
            throw std::runtime_error("OneOfConcept::evaluate - no object with name of constant exists in instance: (" + m_constant.get_name_ref() + ")");
        }
        ConceptDenotation result(state.get_instance_info_ref().get_num_objects());
        compute_result(state, result);
        return result;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(" << m_constant.get_name_ref() << ")";
    }

    static std::string get_name() {
        return "c_one_of";
    }
};

}

#endif
