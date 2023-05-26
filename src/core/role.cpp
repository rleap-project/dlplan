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

const RoleDenotation* Role::evaluate(const State& state, DenotationsCaches& caches) const {
    auto cached = caches.get_role_denotation(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : get_index());
    if (cached) return cached;
    const RoleDenotation* denotation = caches.insert(evaluate_impl(state, caches));
    caches.insert(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : get_index(),
        denotation);
    return denotation;
}

const RoleDenotations* Role::evaluate(const States& states, DenotationsCaches& caches) const {
    auto cached = caches.get_role_denotations(get_index());
    if (cached) return cached;
    auto denotations = evaluate_impl(states, caches);
    auto result_denotations = caches.insert(std::move(denotations));
    caches.insert(get_index(), result_denotations);
    return result_denotations;
}

}
