#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPT_H_

#include "element.h"


namespace dlplan::core::element {

class Concept : public Element<ConceptDenotation> {
protected:
    virtual std::unique_ptr<ConceptDenotation> evaluate_impl(const State& state, DenotationsCaches& caches) const = 0;
    virtual std::unique_ptr<ConceptDenotations> evaluate_impl(const States& states, DenotationsCaches& caches) const = 0;

public:
    Concept(const VocabularyInfo& vocabulary, bool is_static) : Element<ConceptDenotation>(vocabulary, is_static) { }
    ~Concept() override = default;

    /**
     * Evaluate without caching for a single state.
     */
    virtual ConceptDenotation evaluate(const State& state) const = 0;

    /**
     * Evaluate with caching for a single state.
     */
    ConceptDenotation* evaluate(const State& state, DenotationsCaches& caches) const {
        if (is_static()) {
            // check if denotations is cached.
            std::array<int, 2> key({state.get_instance_info()->get_index(), get_index()});
            auto cached = caches.m_c_denots_mapping_per_instance.find(key);
            if (cached != caches.m_c_denots_mapping_per_instance.end()) return cached->second;
            // compute denotation
            auto denotation = evaluate_impl(state, caches);
            // register denotation and append it to denotations.
            auto result_denotation = caches.m_c_denot_cache.insert(std::move(denotation)).first->get();
            caches.m_c_denots_mapping_per_instance.emplace(key, result_denotation);
            return result_denotation;
        } else {
            // check if denotations is cached.
            std::array<int, 3> key({state.get_instance_info()->get_index(), state.get_index(), get_index()});
            auto cached = caches.m_c_denots_mapping_per_state.find(key);
            if (cached != caches.m_c_denots_mapping_per_state.end()) return cached->second;
            // compute denotation
            auto denotation = evaluate_impl(state, caches);
            // register denotation and append it to denotations.
            auto result_denotation = caches.m_c_denot_cache.insert(std::move(denotation)).first->get();
            caches.m_c_denots_mapping_per_state.emplace(key, result_denotation);
            return result_denotation;
        }
    }

    /**
     * Evaluate with caching for a collection of states.
     */
    ConceptDenotations* evaluate(const States& states, DenotationsCaches& caches) const {
        // check if denotations is cached.
        auto cached = caches.m_c_denots_mapping.find(get_index());
        if (cached != caches.m_c_denots_mapping.end()) return cached->second;
        // compute denotations
        auto denotations = evaluate_impl(states, caches);
        // register denotations and return it.
        auto result_denotations = caches.m_c_denots_cache.insert(std::move(denotations)).first->get();
        caches.m_c_denots_mapping.emplace(get_index(), result_denotations);
        return result_denotations;
    }
};

}

#endif
