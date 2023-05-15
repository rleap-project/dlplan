#include "../../include/dlplan/core.h"


namespace dlplan::core {


Numerical::Numerical(std::shared_ptr<const VocabularyInfo> vocabulary_info, bool is_static)
    : BaseElement(vocabulary_info, is_static) {
}

Numerical::Numerical(const Numerical& other) = default;

Numerical& Numerical::operator=(const Numerical& other) = default;

Numerical::Numerical(Numerical&& other) = default;

Numerical& Numerical::operator=(Numerical&& other) = default;

Numerical::~Numerical() = default;

int Numerical::evaluate(const State& state, DenotationsCaches& caches) const {
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

NumericalDenotations* Numerical::evaluate(const States& states, DenotationsCaches& caches) const {
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


}
