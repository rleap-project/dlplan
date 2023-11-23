#include "../../../../include/dlplan/core/elements/concepts/equal.h"


namespace dlplan::core {
void EqualConcept::compute_result(const RoleDenotation& left_denot, const RoleDenotation& right_denot, ConceptDenotation& result) const {
    // find counterexample [(a,b) in R and (a,b) not in S] or [(a,b) not in R and (a,b) in S]
    result.set();
    for (const auto& pair : left_denot.to_vector()) {
        if (!right_denot.contains(pair)) result.erase(pair.first);
    }
    for (const auto& pair : right_denot.to_vector()) {
        if (!left_denot.contains(pair)) result.erase(pair.first);
    }
}

ConceptDenotation EqualConcept::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_role_left->evaluate(state, caches),
        *m_role_right->evaluate(state, caches),
        denotation);
    return denotation;
}

ConceptDenotations EqualConcept::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    ConceptDenotations denotations;
    denotations.reserve(states.size());
    auto role_left_denotations = m_role_left->evaluate(states, caches);
    auto role_right_denotations = m_role_right->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*role_left_denotations)[i],
            *(*role_right_denotations)[i],
            denotation);
        // register denotation and append it to denotations.
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

EqualConcept::EqualConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role_left, std::shared_ptr<const Role> role_right)
    : Concept(index, vocabulary_info, role_left->is_static() && role_right->is_static()),
        m_role_left(role_left), m_role_right(role_right) { }

bool EqualConcept::are_equal_impl(const Concept& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const EqualConcept&>(other);
        return m_is_static == other_derived.m_is_static
            && m_role_left == other_derived.m_role_left
            && m_role_right == other_derived.m_role_right;
    }
    return false;
}

size_t EqualConcept::hash_impl() const {
    return hash_combine(m_is_static, m_role_left, m_role_right);
}

ConceptDenotation EqualConcept::evaluate(const State& state) const {
    auto denotation = ConceptDenotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_role_left->evaluate(state),
        m_role_right->evaluate(state),
        denotation);
    return denotation;
}

int EqualConcept::compute_complexity_impl() const {
    return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
}

void EqualConcept::str_impl(std::stringstream& out) const {
    out << "c_equal" << "(";
    m_role_left->str(out);
    out << ",";
    m_role_right->str(out);
    out << ")";
}

int EqualConcept::compute_evaluate_time_score_impl() const {
    return m_role_left->compute_evaluate_time_score() + m_role_right->compute_evaluate_time_score() + SCORE_QUADRATIC;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::EqualConcept>>::operator()(
        const std::shared_ptr<const dlplan::core::EqualConcept>& left_concept,
        const std::shared_ptr<const dlplan::core::EqualConcept>& right_concept) const {
        return *left_concept < *right_concept;
    }

    std::size_t hash<dlplan::core::EqualConcept>::operator()(const dlplan::core::EqualConcept& concept_) const {
        return concept_.hash();
    }
}