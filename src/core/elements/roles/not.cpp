#include "../../../../include/dlplan/core/elements/roles/not.h"


namespace dlplan::core {
void NotRole::compute_result(const RoleDenotation& denot, RoleDenotation& result) const {
    result = denot;
    ~result;
}

RoleDenotation NotRole::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_role->evaluate(state, caches),
        denotation);
    return denotation;
}

RoleDenotations NotRole::evaluate_impl(const States& states, DenotationsCaches& caches) const {
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

NotRole::NotRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role)
    : Role(index, vocabulary_info, role->is_static()), m_role(role) { }

bool NotRole::are_equal_impl(const Role& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const NotRole&>(other);
        return m_is_static == other_derived.m_is_static
            && m_role == other_derived.m_role;
    }
    return false;
}

size_t NotRole::hash_impl() const {
    return hash_combine(m_is_static, m_role);
}

RoleDenotation NotRole::evaluate(const State& state) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_role->evaluate(state),
        denotation);
    return denotation;
}

int NotRole::compute_complexity_impl() const {
    return m_role->compute_complexity() + 1;
}

void NotRole::str_impl(std::stringstream& out) const {
    out << "r_not" << "(";
    m_role->str(out);
    out << ")";
}

int NotRole::compute_evaluate_time_score_impl() const {
    return m_role->compute_evaluate_time_score() + SCORE_QUADRATIC;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::NotRole>>::operator()(
        const std::shared_ptr<const dlplan::core::NotRole>& left_role,
        const std::shared_ptr<const dlplan::core::NotRole>& right_role) const {
        return *left_role < *right_role;
    }

    std::size_t hash<dlplan::core::NotRole>::operator()(const dlplan::core::NotRole& role) const {
        return role.hash();
    }
}
