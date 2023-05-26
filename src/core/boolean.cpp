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
    auto cached = caches.get_boolean_denotation(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : get_index());
    if (cached) return cached;
    bool denotation = evaluate_impl(state, caches);
    caches.insert(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : get_index(),
        denotation);
    return denotation;
}

const BooleanDenotations* Boolean::evaluate(const States& states, DenotationsCaches& caches) const {
    auto cached = caches.get_boolean_denotations(get_index());
    if (cached) return cached;
    auto denotations = evaluate_impl(states, caches);
    auto result_denotations = caches.insert(std::move(denotations));
    caches.insert(get_index(), result_denotations);
    return result_denotations;
}

}
