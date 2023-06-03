#include "../../include/dlplan/core.h"


namespace dlplan::core {


Boolean::Boolean(std::shared_ptr<const VocabularyInfo> vocabulary_info, bool is_static)
    : BaseElement(vocabulary_info, is_static) {
}

Boolean::Boolean(const Boolean& other) = default;

Boolean& Boolean::operator=(const Boolean& other) = default;

Boolean::Boolean(Boolean&& other) = default;

Boolean& Boolean::operator=(Boolean&& other) = default;

Boolean::~Boolean() = default;

bool Boolean::evaluate(const State& state, DenotationsCaches& caches) const {
    const bool* cached = caches.get_boolean_denotation_cache().get_denotation(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : state.get_index());
    if (cached) return *cached;
    const bool* denotation = caches.get_boolean_denotation_cache().insert_denotation(evaluate_impl(state, caches));
    caches.get_boolean_denotation_cache().insert_denotation(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : state.get_index(),
        denotation);
    return *denotation;
}

const BooleanDenotations* Boolean::evaluate(const States& states, DenotationsCaches& caches) const {
    auto cached = caches.get_boolean_denotations_cache().get_denotation(get_index(), -1, -1);
    if (cached) return cached;
    auto result_denotations = caches.get_boolean_denotations_cache().insert_denotation(evaluate_impl(states, caches));
    caches.get_boolean_denotations_cache().insert_denotation(get_index(), -1, -1, result_denotations);
    return result_denotations;
}

}
