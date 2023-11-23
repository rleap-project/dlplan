#include "../../../../include/dlplan/core/elements/numericals/concept_distance.h"


namespace dlplan::core {
void ConceptDistanceNumerical::compute_result(const ConceptDenotation& concept_from_denot, const RoleDenotation& role_denot, const ConceptDenotation& concept_to_denot, int& result) const {
    result = utils::compute_multi_source_multi_target_shortest_distance(concept_from_denot, role_denot, concept_to_denot);
}

int ConceptDistanceNumerical::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    auto concept_from_denot = m_concept_from->evaluate(state, caches);
    if (concept_from_denot->empty()) {
        return INF;
    }
    auto concept_to_denot = m_concept_to->evaluate(state, caches);
    if (concept_to_denot->empty()) {
        return INF;
    }
    if (concept_from_denot->intersects(*concept_to_denot)) {
        return 0;
    }
    auto role_denot = m_role->evaluate(state, caches);
    int denotation;
    compute_result(
        *concept_from_denot,
        *role_denot,
        *concept_to_denot, denotation);
    return denotation;
}

NumericalDenotations ConceptDistanceNumerical::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    NumericalDenotations denotations;
    denotations.reserve(states.size());
    auto concept_from_denots = m_concept_from->evaluate(states, caches);
    auto role_denots = m_role->evaluate(states, caches);
    auto concept_to_denots = m_concept_to->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        if ((*concept_from_denots)[i]->empty()) {
            denotations.push_back(INF);
            continue;
        }
        if ((*concept_to_denots)[i]->empty()) {
            denotations.push_back(INF);
            continue;
        }
        if ((*concept_from_denots)[i]->intersects(*(*concept_to_denots)[i])) {
            denotations.push_back(0);
            continue;
        }
        int denotation;
        compute_result(
            *(*concept_from_denots)[i],
            *(*role_denots)[i],
            *(*concept_to_denots)[i],
            denotation);
        denotations.push_back(denotation);
    }
    return denotations;
}

ConceptDistanceNumerical::ConceptDistanceNumerical(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Concept> concept_from, std::shared_ptr<const Role> role, std::shared_ptr<const Concept> concept_to)
    : Numerical(index, vocabulary_info, concept_from->is_static() && role->is_static() && concept_to->is_static()),
        m_concept_from(concept_from), m_role(role), m_concept_to(concept_to) { }

bool ConceptDistanceNumerical::are_equal_impl(const Numerical& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const ConceptDistanceNumerical&>(other);
        return m_is_static == other_derived.m_is_static
            && m_concept_from == other_derived.m_concept_from
            && m_role == other_derived.m_role
            && m_concept_to == other_derived.m_concept_to;
    }
    return false;
}

size_t ConceptDistanceNumerical::hash_impl() const {
    return hash_combine(m_is_static, m_concept_from, m_role, m_concept_to);
}

int ConceptDistanceNumerical::evaluate(const State& state) const {
    auto concept_from_denot = m_concept_from->evaluate(state);
    if (concept_from_denot.empty()) {
        return INF;
    }
    auto concept_to_denot = m_concept_to->evaluate(state);
    if (concept_to_denot.empty()) {
        return INF;
    }
    if (concept_from_denot.intersects(concept_to_denot)) {
        return 0;
    }
    auto role_denot = m_role->evaluate(state);
    int denotation;
    compute_result(concept_from_denot, role_denot, concept_to_denot, denotation);
    return denotation;
}

int ConceptDistanceNumerical::compute_complexity_impl() const {
    return m_concept_from->compute_complexity() + m_role->compute_complexity() + m_concept_to->compute_complexity() + 1;
}

void ConceptDistanceNumerical::str_impl(std::stringstream& out) const {
    out << "n_concept_distance" << "(";
    m_concept_from->str(out);
    out << ",";
    m_role->str(out);
    out << ",";
    m_concept_to->str(out);
    out << ")";
}

int ConceptDistanceNumerical::compute_evaluate_time_score_impl() const {
    return m_concept_from->compute_evaluate_time_score() + m_role->compute_evaluate_time_score() + m_concept_to->compute_evaluate_time_score() + SCORE_QUBIC;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::ConceptDistanceNumerical>>::operator()(
        const std::shared_ptr<const dlplan::core::ConceptDistanceNumerical>& left_numerical,
        const std::shared_ptr<const dlplan::core::ConceptDistanceNumerical>& right_numerical) const {
        return *left_numerical < *right_numerical;
    }

    std::size_t hash<dlplan::core::ConceptDistanceNumerical>::operator()(const dlplan::core::ConceptDistanceNumerical& numerical) const {
        return numerical.hash();
    }
}
