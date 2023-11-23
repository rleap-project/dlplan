#include "../../../../include/dlplan/core/elements/roles/inverse.h"


namespace dlplan::core {
void InverseRole::compute_result(const RoleDenotation& denot, RoleDenotation& result) const {
    for (const auto& pair : denot.to_vector()) {
        result.insert(std::make_pair(pair.second, pair.first));
    }
}

RoleDenotation InverseRole::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_role->evaluate(state, caches),
        denotation);
    return denotation;
}

RoleDenotations InverseRole::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    RoleDenotations denotations;
    denotations.reserve(states.size());
    auto role_denotations = m_role->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*role_denotations)[i],
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

InverseRole::InverseRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role)
    : Role(index, vocabulary_info, role->is_static()), m_role(role) { }

bool InverseRole::are_equal_impl(const Role& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const InverseRole&>(other);
        return m_is_static == other_derived.m_is_static
            && m_role == other_derived.m_role;
    }
    return false;
}

size_t InverseRole::hash_impl() const {
    return hash_combine(m_is_static, m_role);
}

RoleDenotation InverseRole::evaluate(const State& state) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_role->evaluate(state),
        denotation);
    return denotation;
}

int InverseRole::compute_complexity_impl() const {
    return m_role->compute_complexity() + 1;
}

void InverseRole::str_impl(std::stringstream& out) const {
    out << "r_inverse" << "(";
    m_role->str(out);
    out << ")";
}

int InverseRole::compute_evaluate_time_score_impl() const {
    return m_role->compute_evaluate_time_score() + SCORE_QUADRATIC;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::InverseRole>>::operator()(
        const std::shared_ptr<const dlplan::core::InverseRole>& left_role,
        const std::shared_ptr<const dlplan::core::InverseRole>& right_role) const {
        return *left_role < *right_role;
    }

    std::size_t hash<dlplan::core::InverseRole>::operator()(const dlplan::core::InverseRole& role) const {
        return role.hash();
    }
}
