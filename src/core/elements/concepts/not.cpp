#include "../../../../include/dlplan/core/elements/concepts/not.h"



namespace dlplan::core {
void NotConcept::compute_result(const ConceptDenotation& denot, ConceptDenotation& result) const {
    result = denot;
    ~result;
}

ConceptDenotation NotConcept::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_concept->evaluate(state, caches),
        denotation);
    return denotation;
}

ConceptDenotations NotConcept::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    ConceptDenotations denotations;
    denotations.reserve(states.size());
    // get denotations of children
    auto concept_denotations = m_concept->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*concept_denotations)[i],
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

NotConcept::NotConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Concept> concept_)
    : Concept(index, vocabulary_info, concept_->is_static()), m_concept(concept_){ }

bool NotConcept::are_equal_impl(const Concept& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const NotConcept&>(other);
        return m_is_static == other_derived.m_is_static
            && m_concept == other_derived.m_concept;
    }
    return false;
}

size_t NotConcept::hash_impl() const {
    return hash_combine(m_is_static, m_concept);
}

ConceptDenotation NotConcept::evaluate(const State& state) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_concept->evaluate(state),
        denotation);
    return denotation;
}

int NotConcept::compute_complexity_impl() const {
    return m_concept->compute_complexity() + 1;
}

void NotConcept::str_impl(std::stringstream& out) const {
    out << "c_not" << "(";
    m_concept->str(out);
    out << ")";
}

int NotConcept::compute_evaluate_time_score_impl() const {
    return m_concept->compute_evaluate_time_score() + SCORE_LINEAR;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::NotConcept>>::operator()(
        const std::shared_ptr<const dlplan::core::NotConcept>& left_concept,
        const std::shared_ptr<const dlplan::core::NotConcept>& right_concept) const {
        return *left_concept < *right_concept;
    }

    std::size_t hash<dlplan::core::NotConcept>::operator()(const dlplan::core::NotConcept& concept_) const {
        return concept_.hash();
    }
}