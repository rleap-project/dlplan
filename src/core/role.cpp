#include "../../include/dlplan/core.h"


namespace dlplan::core {

Role::Role(std::shared_ptr<const VocabularyInfo> vocabulary_info, bool is_static)
    : BaseElement(vocabulary_info, is_static) {
}

Role::Role(const Role& other) = default;

Role& Role::operator=(const Role& other) = default;

Role::Role(Role&& other) = default;

Role& Role::operator=(Role&& other) = default;

Role::~Role() = default;

RoleDenotation* Role::evaluate(const State& state, DenotationsCaches& caches) const {
    if (is_static()) {
        // check if denotations is cached.
        std::array<int, 2> key({state.get_instance_info()->get_index(), get_index()});
        auto cached = caches.m_r_denots_mapping_per_instance.find(key);
        if (cached != caches.m_r_denots_mapping_per_instance.end()) return cached->second;
        // compute denotation
        auto denotation = evaluate_impl(state, caches);
        // register denotation and append it to denotations.
        auto result_denotation = caches.m_r_denot_cache.insert(std::move(denotation)).first->get();
        caches.m_r_denots_mapping_per_instance.emplace(key, result_denotation);
        return result_denotation;
    } else {
        // check if denotations is cached.
        std::array<int, 3> key({state.get_instance_info()->get_index(), state.get_index(), get_index()});
        auto cached = caches.m_r_denots_mapping_per_state.find(key);
        if (cached != caches.m_r_denots_mapping_per_state.end()) return cached->second;
        // compute denotation
        auto denotation = evaluate_impl(state, caches);
        // register denotation and append it to denotations.
        auto result_denotation = caches.m_r_denot_cache.insert(std::move(denotation)).first->get();
        caches.m_r_denots_mapping_per_state.emplace(key, result_denotation);
        return result_denotation;
    }
}

RoleDenotations* Role::evaluate(const States& states, DenotationsCaches& caches) const {
    // check if denotations is cached.
    auto cached = caches.m_r_denots_mapping.find(get_index());
    if (cached != caches.m_r_denots_mapping.end()) return cached->second;
    // compute denotations
    auto denotations = evaluate_impl(states, caches);
    // register denotations and return it.
    auto result_denotations = caches.m_r_denots_cache.insert(std::move(denotations)).first->get();
    caches.m_r_denots_mapping.emplace(get_index(), result_denotations);
    return result_denotations;
}

}
