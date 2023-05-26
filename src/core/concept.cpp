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
    auto cached = caches.get_denotation<ConceptDenotation>(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : get_index());
    if (cached) return cached;
    const ConceptDenotation* denotation = caches.insert_denotation(evaluate_impl(state, caches));
    caches.insert_denotation(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : get_index(),
        denotation);
    return denotation;
}

const ConceptDenotations* Concept::evaluate(const States& states, DenotationsCaches& caches) const {
    auto cached = caches.get_denotations<ConceptDenotations>(get_index());
    if (cached) return cached;
    auto denotations = evaluate_impl(states, caches);
    auto result_denotations = caches.insert_denotation(std::move(denotations));
    caches.insert_denotations(get_index(), result_denotations);
    return result_denotations;
}

}
