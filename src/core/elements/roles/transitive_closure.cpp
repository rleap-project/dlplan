#include "../../../../include/dlplan/core/elements/roles/transitive_closure.h"


namespace dlplan::core {
// https://stackoverflow.com/questions/3517524/what-is-the-best-known-transitive-closure-algorithm-for-a-directed-graph
void TransitiveClosureRole::compute_result(const RoleDenotation& denot, RoleDenotation& result) const {
    result = denot;
    bool changed = false;
    do {
        RoleDenotation tmp_result = result;
        PairsOfObjectIndices pairs = tmp_result.to_vector();
        for (const auto& pair_1 : pairs) {
            for (const auto& pair_2 : pairs) {
                if (pair_1.second == pair_2.first) {
                    result.insert(std::make_pair(pair_1.first, pair_2.second));
                }
            }
        }
        changed = (result.size() != tmp_result.size());
    } while (changed);
}

RoleDenotation TransitiveClosureRole::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_role->evaluate(state, caches),
        denotation);
    return denotation;
}

RoleDenotations TransitiveClosureRole::evaluate_impl(const States& states, DenotationsCaches& caches) const {
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

TransitiveClosureRole::TransitiveClosureRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role)
    : Role(index, vocabulary_info, role->is_static()), m_role(role) { }

bool TransitiveClosureRole::are_equal_impl(const Role& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const TransitiveClosureRole&>(other);
        return m_is_static == other_derived.m_is_static
            && m_role == other_derived.m_role;
    }
    return false;
}

size_t TransitiveClosureRole::hash_impl() const {
    return hash_combine(m_is_static, m_role);
}

RoleDenotation TransitiveClosureRole::evaluate(const State& state) const {
    RoleDenotation result(state.get_instance_info()->get_objects().size());
    compute_result(
        m_role->evaluate(state),
        result);
    return result;
}

int TransitiveClosureRole::compute_complexity_impl() const {
    return m_role->compute_complexity() + 1;
}

void TransitiveClosureRole::str_impl(std::stringstream& out) const {
    out << "r_transitive_closure" << "(";
    m_role->str(out);
    out << ")";
}

int TransitiveClosureRole::compute_evaluate_time_score_impl() const {
    return m_role->compute_evaluate_time_score() + SCORE_QUBIC;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::TransitiveClosureRole>>::operator()(
        const std::shared_ptr<const dlplan::core::TransitiveClosureRole>& left_role,
        const std::shared_ptr<const dlplan::core::TransitiveClosureRole>& right_role) const {
        return *left_role < *right_role;
    }

    std::size_t hash<dlplan::core::TransitiveClosureRole>::operator()(const dlplan::core::TransitiveClosureRole& role) const {
        return role.hash();
    }
}
