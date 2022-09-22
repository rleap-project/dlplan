#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_

#include "../concept.h"


namespace dlplan::core::element {

class OneOfConcept : public Concept {
private:
    void compute_result(const State& state, ConceptDenotation& result) const {
        result.insert(state.get_instance_info_ref().get_object_idx(m_constant.get_name_ref()));
    }

protected:
    const Constant m_constant;

public:
    OneOfConcept(const VocabularyInfo& vocabulary, const Constant& constant)
    : Concept(vocabulary), m_constant(constant) {
    }

    ConceptDenotation evaluate(const State& state) const override {
        if (!state.get_instance_info_ref().exists_object(m_constant.get_name_ref())) {
            throw std::runtime_error("OneOfConcept::evaluate - no object with name of constant exists in instance: (" + m_constant.get_name_ref() + ")");
        }
        ConceptDenotation result(state.get_instance_info_ref().get_num_objects());
        compute_result(state, result);
        return result;
    }

    DENOTS<ConceptDenotation*>* evaluate(const States& states, DenotationsCaches& caches) const override {
        auto cached = caches.m_c_denots_cache.find(get_index());
        if (cached) return cached;
        auto denotations = caches.m_c_denots_cache.get_new_denotations();
        for (size_t i = 0; i < states.size(); ++i) {
            const auto& state = states[i];
            int num_objects = state.get_instance_info_ref().get_num_objects();
            auto denotation = caches.m_c_denot_cache.get_new_denotation(num_objects);
            compute_result(
                state,
                *denotation);
            denotations->push_back(caches.m_c_denot_cache.insert(std::move(denotation)));
        }
        return caches.m_c_denots_cache.insert(std::move(denotations), get_index());
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
