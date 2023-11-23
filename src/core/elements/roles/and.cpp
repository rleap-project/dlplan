#include "../../../../include/dlplan/core/elements/roles/and.h"


namespace dlplan::core {

void AndRole::compute_result(const RoleDenotation& left_denot, const RoleDenotation& right_denot, RoleDenotation& result) const {
    result = left_denot;
    result &= right_denot;
}

RoleDenotation AndRole::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_role_left->evaluate(state, caches),
        *m_role_right->evaluate(state, caches),
        denotation);
    return denotation;
}

RoleDenotations AndRole::evaluate_impl(const States& states, DenotationsCaches& caches) const {
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

AndRole::AndRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role_1, std::shared_ptr<const Role> role_2)
    : Role(index, vocabulary_info, role_1->is_static() && role_2->is_static()),
    m_role_left(role_1->get_index() < role_2->get_index() ? role_1 : role_2),
    m_role_right(role_1->get_index() < role_2->get_index() ? role_2 : role_1) { }

bool AndRole::are_equal_impl(const Role& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const AndRole&>(other);
        return m_is_static == other_derived.m_is_static
            && m_role_left == other_derived.m_role_left
            && m_role_right == other_derived.m_role_right;
    }
    return false;
}

size_t AndRole::hash_impl() const {
    return hash_combine(m_is_static, m_role_left, m_role_right);
}

RoleDenotation AndRole::evaluate(const State& state) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_role_left->evaluate(state),
        m_role_right->evaluate(state),
        denotation);
    return denotation;
}

int AndRole::compute_complexity_impl() const {
    return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
}

void AndRole::str_impl(std::stringstream& out) const {
    out << "r_and" << "(";
    m_role_left->str(out);
    out << ",";
    m_role_right->str(out);
    out << ")";
}

int AndRole::compute_evaluate_time_score_impl() const {
    return m_role_left->compute_evaluate_time_score() + m_role_right->compute_evaluate_time_score() + SCORE_QUADRATIC;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::AndRole>>::operator()(
        const std::shared_ptr<const dlplan::core::AndRole>& left_role,
        const std::shared_ptr<const dlplan::core::AndRole>& right_role) const {
        return *left_role < *right_role;
    }

    std::size_t hash<dlplan::core::AndRole>::operator()(const dlplan::core::AndRole& role) const {
        return role.hash();
    }
}