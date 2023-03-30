#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_H_

#include "element.h"


namespace dlplan::core::element {

class Boolean : public Element<bool> {
protected:
    virtual bool evaluate_impl(const State& state, DenotationsCaches& caches) const = 0;
    virtual std::unique_ptr<BooleanDenotations> evaluate_impl(const States& states, DenotationsCaches& caches) const = 0;

public:
    Boolean(const VocabularyInfo& vocabulary, bool is_static) : Element<bool>(vocabulary, is_static) { }
    ~Boolean() override = default;

    /**
     * Evaluate without caching for a single state.
     */
    virtual bool evaluate(const State& state) const = 0;

    /**
     * Evaluate with caching for a single state.
     */
    bool evaluate(const State& state, DenotationsCaches& caches) const {
        if (is_static()) {
            // check if denotations is cached.
            auto key = std::array<int, 2>({state.get_instance_info()->get_index(), get_index()});
            auto cached = caches.m_b_denots_mapping_per_instance.find(key);
            if (cached != caches.m_b_denots_mapping_per_instance.end()) return cached->second;
            // compute denotation
            bool denotation = evaluate_impl(state, caches);
            // register denotation and return it
            caches.m_b_denots_mapping_per_instance.emplace(key, denotation);
            return denotation;
        } else {
            // check if denotations is cached.
            auto key = std::array<int, 3>({state.get_instance_info()->get_index(), state.get_index(), get_index()});
            auto cached = caches.m_b_denots_mapping_per_state.find(key);
            if (cached != caches.m_b_denots_mapping_per_state.end()) return cached->second;
            // compute denotation
            bool denotation = evaluate_impl(state, caches);
            // register denotation and return it
            caches.m_b_denots_mapping_per_state.emplace(key, denotation);
            return denotation;
        }
    }

    /**
     * Evaluate with caching for a collection of states.
     */
    BooleanDenotations* evaluate(const States& states, DenotationsCaches& caches) const {
        // check if denotations is cached.
        auto cached = caches.m_b_denots_mapping.find(get_index());
        if (cached != caches.m_b_denots_mapping.end()) return cached->second;
        // compute denotations
        auto denotations = evaluate_impl(states, caches);
        // register denotations and return it.
        auto result_denotations = caches.m_b_denots_cache.insert(std::move(denotations)).first->get();
        caches.m_b_denots_mapping.emplace(get_index(), result_denotations);
        return result_denotations;
    }
};

}

#endif
