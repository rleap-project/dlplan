#include "../../../../include/dlplan/core/elements/concepts/top.h"


namespace dlplan::core {
ConceptDenotation TopConcept::evaluate_impl(const State& state, DenotationsCaches&) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    denotation.set();
    return denotation;
}

ConceptDenotations TopConcept::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    ConceptDenotations denotations;
    denotations.reserve(states.size());
    for (size_t i = 0; i < states.size(); ++i) {
        ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
        denotation.set();
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

TopConcept::TopConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info)
    : Concept(index, vocabulary_info, true) {
}

bool TopConcept::are_equal_impl(const Concept& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const TopConcept&>(other);
        return m_is_static == other_derived.m_is_static;
    }
    return false;
}

size_t TopConcept::hash_impl() const {
    return hash_combine(m_is_static);
}

ConceptDenotation TopConcept::evaluate(const State& state) const {
    auto denotation = ConceptDenotation(state.get_instance_info()->get_objects().size());
    denotation.set();
    return denotation;
}

int TopConcept::compute_complexity_impl() const {
    return 1;
}

void TopConcept::str_impl(std::stringstream& out) const {
    out << "c_top";
}

int TopConcept::compute_evaluate_time_score_impl() const {
    return SCORE_CONSTANT;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::TopConcept>>::operator()(
        const std::shared_ptr<const dlplan::core::TopConcept>& left_concept,
        const std::shared_ptr<const dlplan::core::TopConcept>& right_concept) const {
        return *left_concept < *right_concept;
    }

    std::size_t hash<dlplan::core::TopConcept>::operator()(const dlplan::core::TopConcept& concept_) const {
        return concept_.hash();
    }
}
