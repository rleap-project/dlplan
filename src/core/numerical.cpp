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
    const int* cached = caches.get_numerical_denotation_cache().get_denotation(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : state.get_index());
    if (cached) return *cached;
    const int* denotation = caches.get_numerical_denotation_cache().insert_denotation(evaluate_impl(state, caches));
    caches.get_numerical_denotation_cache().insert_denotation(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : state.get_index(),
        denotation);
    return *denotation;
}

const NumericalDenotations* Numerical::evaluate(const States& states, DenotationsCaches& caches) const {
    auto cached = caches.get_numerical_denotations_cache().get_denotation(get_index(), -1, -1);
    if (cached) return cached;
    auto result_denotations = caches.get_numerical_denotations_cache().insert_denotation(evaluate_impl(states, caches));
    caches.get_numerical_denotations_cache().insert_denotation(get_index(), -1, -1, result_denotations);
    return result_denotations;
}

}
