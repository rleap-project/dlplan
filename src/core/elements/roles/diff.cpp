#include "../../../../include/dlplan/core/elements/roles/diff.h"


namespace dlplan::core {
void DiffRole::compute_result(const RoleDenotation& left_denot, const RoleDenotation& right_denot, RoleDenotation& result) const {
    result = left_denot;
    result -= right_denot;
}

RoleDenotation DiffRole::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_role_left->evaluate(state, caches),
        *m_role_right->evaluate(state, caches),
        denotation);
    return denotation;
}

RoleDenotations DiffRole::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    RoleDenotations denotations;
    denotations.reserve(states.size());
    auto role_left_denotations = m_role_left->evaluate(states, caches);
    auto role_right_denotations = m_role_right->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*role_left_denotations)[i],
            *(*role_right_denotations)[i],
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

DiffRole::DiffRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role_left, std::shared_ptr<const Role> role_right)
    : Role(index, vocabulary_info, (role_left->is_static() && role_right->is_static())), m_role_left(role_left), m_role_right(role_right)  { }

bool DiffRole::are_equal_impl(const Role& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const DiffRole&>(other);
        return m_is_static == other_derived.m_is_static
            && m_role_left == other_derived.m_role_left
            && m_role_right == other_derived.m_role_right;
    }
    return false;
}

size_t DiffRole::hash_impl() const {
    return hash_combine(m_is_static, m_role_left, m_role_right);
}

RoleDenotation DiffRole::evaluate(const State& state) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_role_left->evaluate(state),
        m_role_right->evaluate(state),
        denotation);
    return denotation;
}

int DiffRole::compute_complexity_impl() const {
    return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
}

void DiffRole::str_impl(std::stringstream& out) const {
    out << "r_diff" << "(";
    m_role_left->str(out);
    out << ",";
    m_role_right->str(out);
    out << ")";
}

int DiffRole::compute_evaluate_time_score_impl() const {
    return m_role_left->compute_evaluate_time_score() + m_role_right->compute_evaluate_time_score() + SCORE_QUADRATIC;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::DiffRole>>::operator()(
        const std::shared_ptr<const dlplan::core::DiffRole>& left_role,
        const std::shared_ptr<const dlplan::core::DiffRole>& right_role) const {
        return *left_role < *right_role;
    }

    std::size_t hash<dlplan::core::DiffRole>::operator()(const dlplan::core::DiffRole& role) const {
        return role.hash();
    }
}
