#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_TOP_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_TOP_H_

#include "../concept.h"


namespace dlplan::core::element {

class TopConcept : public Concept {
public:
    TopConcept(const VocabularyInfo& vocabulary)
    : Concept(vocabulary) {
    }

    ConceptDenotation evaluate(const State& state) const override {
        return state.get_instance_info_ref().get_top_concept_ref();
    }

    DENOTS<ConceptDenotation*>* evaluate(const States& states, DenotationsCaches& caches) const override {
        // check if denotations is cached.
        auto cached = caches.m_c_denots_mapping.find(get_index());
        if (cached != caches.m_c_denots_mapping.end()) return cached->second;
        // allocate memory for new denotations
        auto denotations = caches.m_c_denots_cache.get_new_entry();
        denotations->reserve(states.size());
        // get denotations of children
        for (size_t i = 0; i < states.size(); ++i) {
            int num_objects = states[i].get_instance_info_ref().get_num_objects();
            auto denotation = caches.m_c_denot_cache.get_new_entry(num_objects);
            denotation->set();
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
        out << get_name();
    }

    static std::string get_name() {
        return "c_top";
    }
};

}

#endif
