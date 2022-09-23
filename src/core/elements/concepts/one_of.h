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

    std::vector<ConceptDenotation*>* evaluate(const States& states, DenotationsCaches& caches) const override {
        // check if denotations is cached.
        auto cached = caches.m_c_denots_mapping.find(get_index());
        if (cached != caches.m_c_denots_mapping.end()) return cached->second;
        // allocate memory for new denotations
        auto denotations = std::make_unique<ConceptDenotations>();
        denotations->reserve(states.size());
        // get denotations of children
        for (size_t i = 0; i < states.size(); ++i) {
            const auto& state = states[i];
            int num_objects = state.get_instance_info_ref().get_num_objects();
            auto denotation = std::make_unique<ConceptDenotation>(ConceptDenotation(num_objects));
            compute_result(
                state,
                *denotation);
            // register denotation and append it to denotations.
            denotations->push_back(caches.m_c_denot_cache.insert(std::move(denotation)).first->get());
        }
        // register denotations and return it.
        auto result_denotations = caches.m_c_denots_cache.insert(std::move(denotations)).first->get();
        caches.m_c_denots_mapping.emplace(get_index(), result_denotations);
        return result_denotations;
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
