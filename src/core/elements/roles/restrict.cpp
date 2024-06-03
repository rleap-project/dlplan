#include "../../../../include/dlplan/core/elements/roles/restrict.h"


namespace dlplan::core {
void RestrictRole::compute_result(const RoleDenotation& role_denot, const ConceptDenotation& concept_denot, RoleDenotation& result) const {
    result = role_denot;
    for (const auto& pair : role_denot.to_vector()) {
        if (!concept_denot.contains(pair.second)) {
            result.erase(pair);
        }
    }
}

RoleDenotation RestrictRole::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_role->evaluate(state, caches),
        *m_concept->evaluate(state, caches),
        denotation);
    return denotation;
}

RoleDenotations RestrictRole::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    RoleDenotations denotations;
    denotations.reserve(states.size());
    auto role_denotations = m_role->evaluate(states, caches);
    auto concept_denotations = m_concept->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*role_denotations)[i],
            *(*concept_denotations)[i],
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

RestrictRole::RestrictRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role, std::shared_ptr<const Concept> concept_)
: Role(index, vocabulary_info, role->is_static() && concept_->is_static()), m_role(role), m_concept(concept_) { }

bool RestrictRole::are_equal_impl(const Role& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const RestrictRole&>(other);
        return m_is_static == other_derived.m_is_static
            && m_role == other_derived.m_role
            && m_concept == other_derived.m_concept;
    }
    return false;
}

size_t RestrictRole::hash_impl() const {
    return hash_combine(m_is_static, m_role, m_concept);
}

RoleDenotation RestrictRole::evaluate(const State& state) const {
    auto role_denot = m_role->evaluate(state);
    auto concept_denot = m_concept->evaluate(state);
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_role->evaluate(state),
        m_concept->evaluate(state),
        denotation);
    return denotation;
}

int RestrictRole::compute_complexity_impl() const {
    return m_role->compute_complexity() + m_concept->compute_complexity() + 1;
}

void RestrictRole::str_impl(std::stringstream& out) const {
    out << "r_restrict" << "(";
    m_role->str(out);
    out << ",";
    m_concept->str(out);
    out << ")";
}

int RestrictRole::compute_evaluate_time_score_impl() const {
    return m_role->compute_evaluate_time_score() + m_concept->compute_evaluate_time_score() + SCORE_QUADRATIC;
}

}


namespace std {
        bool less<std::shared_ptr<const dlplan::core::RestrictRole>>::operator()(
            const std::shared_ptr<const dlplan::core::RestrictRole>& left_role,
            const std::shared_ptr<const dlplan::core::RestrictRole>& right_role) const {
            return *left_role < *right_role;
        }

        std::size_t hash<dlplan::core::RestrictRole>::operator()(const dlplan::core::RestrictRole& role) const {
            return role.hash();
        }
}
