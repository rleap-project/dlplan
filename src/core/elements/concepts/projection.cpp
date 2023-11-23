#include "../../../../include/dlplan/core/elements/concepts/projection.h"


namespace dlplan::core {
void ProjectionConcept::compute_result(const RoleDenotation& denot, ConceptDenotation& result) const {
    for (const auto& pair : denot.to_vector()) {
        if (m_pos == 0) result.insert(pair.first);
        else if (m_pos == 1) result.insert(pair.second);
    }
}

ConceptDenotation ProjectionConcept::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_role->evaluate(state, caches),
        denotation);
    return denotation;
}

ConceptDenotations ProjectionConcept::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    ConceptDenotations denotations;
    denotations.reserve(states.size());
    auto role_denotations = m_role->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*role_denotations)[i],
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

ProjectionConcept::ProjectionConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const std::shared_ptr<const Role>& role, int pos)
: Concept(index, vocabulary_info, role->is_static()), m_role(role), m_pos(pos) {
    if (pos < 0 || pos > 1) {
        throw std::runtime_error("ProjectionConcept::ProjectionConcept - projection index out of range, should be 0 or 1 ("s + std::to_string(pos) + ")");
    }
}

bool ProjectionConcept::are_equal_impl(const Concept& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const ProjectionConcept&>(other);
        return m_is_static == other_derived.m_is_static
            && m_role == other_derived.m_role
            && m_pos == other_derived.m_pos;
    }
    return false;
}

size_t ProjectionConcept::hash_impl() const {
    return hash_combine(m_is_static, m_role, m_pos);
}

ConceptDenotation ProjectionConcept::evaluate(const State& state) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_role->evaluate(state),
        denotation);
    return denotation;
}

int ProjectionConcept::compute_complexity_impl() const {
    return m_role->compute_complexity() + 1;
}

void ProjectionConcept::str_impl(std::stringstream& out) const {
    out << "c_projection" << "(";
    m_role->str(out);
    out << "," << std::to_string(m_pos) << ")";
}

int ProjectionConcept::compute_evaluate_time_score_impl() const {
    return m_role->compute_evaluate_time_score() + SCORE_QUADRATIC;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::ProjectionConcept>>::operator()(
        const std::shared_ptr<const dlplan::core::ProjectionConcept>& left_concept,
        const std::shared_ptr<const dlplan::core::ProjectionConcept>& right_concept) const {
        return *left_concept < *right_concept;
    }

    std::size_t hash<dlplan::core::ProjectionConcept>::operator()(const dlplan::core::ProjectionConcept& concept_) const {
        return concept_.hash();
    }
}
