#include "../../include/dlplan/core.h"


namespace dlplan::core {


Concept::Concept(std::shared_ptr<const VocabularyInfo> vocabulary_info, bool is_static)
    : BaseElement(vocabulary_info, is_static) {
}

Concept::Concept(const Concept& other) = default;

Concept& Concept::operator=(const Concept& other) = default;

Concept::Concept(Concept&& other) = default;

Concept& Concept::operator=(Concept&& other) = default;

Concept::~Concept() = default;

const ConceptDenotation* Concept::evaluate(const State& state, DenotationsCaches& caches) const {
    auto cached = caches.get_concept_denotation_cache().get_denotation(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : state.get_index());
    if (cached) return cached;
    auto denotation = caches.get_concept_denotation_cache().insert_denotation(evaluate_impl(state, caches));
    caches.get_concept_denotation_cache().insert_denotation(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : state.get_index(),
        denotation);
    return denotation;
}

const ConceptDenotations* Concept::evaluate(const States& states, DenotationsCaches& caches) const {
    auto cached = caches.get_concept_denotations_cache().get_denotation(get_index(), -1, -1);
    if (cached) return cached;
    auto result_denotations = caches.get_concept_denotations_cache().insert_denotation(evaluate_impl(states, caches));
    caches.get_concept_denotations_cache().insert_denotation(get_index(), -1, -1, result_denotations);
    return result_denotations;
}

}
