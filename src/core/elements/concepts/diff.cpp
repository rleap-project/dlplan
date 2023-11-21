#include "../../../../include/dlplan/core/elements/concepts/diff.h"


namespace dlplan::core {
void DiffConcept::compute_result(const ConceptDenotation& left_denot, const ConceptDenotation& right_denot, ConceptDenotation& result) const {
    result = left_denot;
    result -= right_denot;
}

ConceptDenotation DiffConcept::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_concept_left->evaluate(state, caches),
        *m_concept_right->evaluate(state, caches),
        denotation);
    return denotation;
}

ConceptDenotations DiffConcept::evaluate_impl(const States& states, DenotationsCaches& caches) const {
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
        denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
    }
    return denotations;
}

DiffConcept::DiffConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Concept> concept_1, std::shared_ptr<const Concept> concept_2)
    : Concept(vocabulary_info, index, concept_1->is_static() && concept_2->is_static()), m_concept_left(concept_1), m_concept_right(concept_2) { }

bool DiffConcept::operator==(const Concept& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const DiffConcept&>(other);
        return m_is_static == other_derived.m_is_static
            && m_concept_left == other_derived.m_concept_left
            && m_concept_right == other_derived.m_concept_right;
    }
    return false;
}

size_t DiffConcept::hash() const {
    return hash_combine(m_is_static, m_concept_left, m_concept_right);
}

ConceptDenotation DiffConcept::evaluate(const State& state) const {
    ConceptDenotation result(state.get_instance_info()->get_objects().size());
    compute_result(
        m_concept_left->evaluate(state),
        m_concept_right->evaluate(state),
        result);
    return result;
}

int DiffConcept::compute_complexity() const {
    return m_concept_left->compute_complexity() + m_concept_right->compute_complexity() + 1;
}

void DiffConcept::compute_repr(std::stringstream& out) const {
    out << "c_diff" << "(";
    m_concept_left->compute_repr(out);
    out << ",";
    m_concept_right->compute_repr(out);
    out << ")";
}

int DiffConcept::compute_evaluate_time_score() const {
    return m_concept_left->compute_evaluate_time_score() + m_concept_right->compute_evaluate_time_score() + SCORE_LINEAR;
}

}


namespace std {
        bool less<std::shared_ptr<const dlplan::core::DiffConcept>>::operator()(
            const std::shared_ptr<const dlplan::core::DiffConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::DiffConcept>& right_concept) const {
            return *left_concept < *right_concept;
        }

        std::size_t hash<dlplan::core::DiffConcept>::operator()(const dlplan::core::DiffConcept& concept_) const {
            return concept_.hash();
        }
}