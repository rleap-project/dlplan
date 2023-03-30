#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_H_

#include "element.h"


namespace dlplan::core::element {

class Numerical : public Element<int> {
protected:
    virtual int evaluate_impl(const State& state, DenotationsCaches& caches) const = 0;
    virtual std::unique_ptr<NumericalDenotations> evaluate_impl(const States& states, DenotationsCaches& caches) const = 0;

public:
    Numerical(const VocabularyInfo& vocabulary, bool is_static) : Element<int>(vocabulary, is_static) { }
    ~Numerical() override = default;

    /**
     * Evaluate without caching for a single state.
     */
    virtual int evaluate(const State& state) const = 0;

    /**
     * Evaluate with caching for a single state.
     */
    int evaluate(const State& state, DenotationsCaches& caches) const {
        if (is_static()) {
            // check if denotations is cached.
            auto key = std::array<int, 2>({state.get_instance_info()->get_index(), get_index()});
            auto cached = caches.m_n_denots_mapping_per_instance.find(key);
            if (cached != caches.m_n_denots_mapping_per_instance.end()) return cached->second;
            // compute denotation
            auto denotation = evaluate_impl(state, caches);
            // register denotation and return it
            caches.m_n_denots_mapping_per_instance.emplace(key, denotation);
            return denotation;
        } else {
            // check if denotations is cached.
            auto key = std::array<int, 3>({state.get_instance_info()->get_index(), state.get_index(), get_index()});
            auto cached = caches.m_n_denots_mapping_per_state.find(key);
            if (cached != caches.m_n_denots_mapping_per_state.end()) return cached->second;
            // compute denotation
            auto denotation = evaluate_impl(state, caches);
            // register denotation and return it
            caches.m_n_denots_mapping_per_state.emplace(key, denotation);
            return denotation;
        }
    }

    /**
     * Evaluate with caching for a collection of states.
     */
    NumericalDenotations* evaluate(const States& states, DenotationsCaches& caches) const {
        // check if denotations is cached.
        auto cached = caches.m_n_denots_mapping.find(get_index());
        if (cached != caches.m_n_denots_mapping.end()) return cached->second;
        // compute denotations
        auto denotations = evaluate_impl(states, caches);
        // register denotations and return it.
        auto result_denotations = caches.m_n_denots_cache.insert(std::move(denotations)).first->get();
        caches.m_n_denots_mapping.emplace(get_index(), result_denotations);
        return result_denotations;
    }
};

}

#endif
