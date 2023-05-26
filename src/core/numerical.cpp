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
    auto cached = caches.get_numerical_denotation(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : get_index());
    if (cached) return cached;
    int denotation = evaluate_impl(state, caches);
    caches.insert(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : get_index(),
        denotation);
    return denotation;
}

const NumericalDenotations* Numerical::evaluate(const States& states, DenotationsCaches& caches) const {
    auto cached = caches.get_numerical_denotations(get_index());
    if (cached) return cached;
    auto denotations = evaluate_impl(states, caches);
    auto result_denotations = caches.insert(std::move(denotations));
    caches.insert(get_index(), result_denotations);
    return result_denotations;
}

}
