#include "../../../../include/dlplan/core/elements/concepts/and.h"


namespace dlplan::core {
void AndConcept::compute_result(const ConceptDenotation& left_denot, const ConceptDenotation& right_denot, ConceptDenotation& result) const {
    result = left_denot;
    result &= right_denot;
}

ConceptDenotation AndConcept::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    denotation.set();
    compute_result(
        *m_concept_left->evaluate(state, caches),
        *m_concept_right->evaluate(state, caches),
        denotation);
    return denotation;
}

ConceptDenotations AndConcept::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    ConceptDenotations denotations;
    denotations.reserve(states.size());
    auto concept_left_denotations = m_concept_left->evaluate(states, caches);
    auto concept_right_denotations = m_concept_right->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*concept_left_denotations)[i],
            *(*concept_right_denotations)[i],
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

AndConcept::AndConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Concept> concept_1, std::shared_ptr<const Concept> concept_2)
    : Concept(index, vocabulary_info, concept_1->is_static() && concept_2->is_static()),
    m_concept_left(concept_1->get_index() < concept_2->get_index() ? concept_1 : concept_2),
    m_concept_right(concept_1->get_index() < concept_2->get_index() ? concept_2 : concept_1) { }

bool AndConcept::are_equal_impl(const Concept& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const AndConcept&>(other);
        return m_is_static == other_derived.m_is_static
            && m_concept_left == other_derived.m_concept_left
            && m_concept_right == other_derived.m_concept_right;
    }
    return false;
}

size_t AndConcept::hash_impl() const {
    return hash_combine(m_is_static, m_concept_left, m_concept_right);
}

ConceptDenotation AndConcept::evaluate(const State& state) const {
    ConceptDenotation result(state.get_instance_info()->get_objects().size());
    compute_result(
        m_concept_left->evaluate(state),
        m_concept_right->evaluate(state),
        result);
    return result;
}

int AndConcept::compute_complexity_impl() const {
    return m_concept_left->compute_complexity() + m_concept_right->compute_complexity() + 1;
}

void AndConcept::str_impl(std::stringstream& out) const {
    out << "c_and" << "(";
    m_concept_left->str(out);
    out << ",";
    m_concept_right->str(out);
    out << ")";
}

int AndConcept::compute_evaluate_time_score_impl() const {
    return m_concept_left->compute_evaluate_time_score() + m_concept_right->compute_evaluate_time_score() + SCORE_LINEAR;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::AndConcept>>::operator()(
        const std::shared_ptr<const dlplan::core::AndConcept>& left_concept,
        const std::shared_ptr<const dlplan::core::AndConcept>& right_concept) const {
        return *left_concept < *right_concept;
    }

    std::size_t hash<dlplan::core::AndConcept>::operator()(const dlplan::core::AndConcept& concept_) const {
        return concept_.hash();
    }
}

