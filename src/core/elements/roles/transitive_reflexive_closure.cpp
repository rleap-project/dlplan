#include "../../../../include/dlplan/core/elements/roles/transitive_reflexive_closure.h"


namespace dlplan::core {
void TransitiveReflexiveClosureRole::compute_result(const RoleDenotation& denot, int num_objects, RoleDenotation& result) const {
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
    // add reflexive part
    for (int i = 0; i < num_objects; ++i) {
        result.insert(std::make_pair(i, i));
    }
}

RoleDenotation TransitiveReflexiveClosureRole::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_role->evaluate(state, caches),
        state.get_instance_info()->get_objects().size(),
        denotation);
    return denotation;
}

RoleDenotations TransitiveReflexiveClosureRole::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    RoleDenotations denotations;
    denotations.reserve(states.size());
    auto role_denotations = m_role->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*role_denotations)[i],
            states[i].get_instance_info()->get_objects().size(),
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

TransitiveReflexiveClosureRole::TransitiveReflexiveClosureRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role)
    : Role(index, vocabulary_info, role->is_static()), m_role(role) { }

bool TransitiveReflexiveClosureRole::are_equal_impl(const Role& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const TransitiveReflexiveClosureRole&>(other);
        return m_is_static == other_derived.m_is_static
            && m_role == other_derived.m_role;
    }
    return false;
}

size_t TransitiveReflexiveClosureRole::hash_impl() const {
    return hash_combine(m_is_static, m_role);
}

RoleDenotation TransitiveReflexiveClosureRole::evaluate(const State& state) const {
    int num_objects = state.get_instance_info()->get_objects().size();
    RoleDenotation denotation(num_objects);
    compute_result(
        m_role->evaluate(state),
        num_objects,
        denotation);
    return denotation;
}

int TransitiveReflexiveClosureRole::compute_complexity_impl() const {
    return m_role->compute_complexity() + 1;
}

void TransitiveReflexiveClosureRole::str_impl(std::stringstream& out) const {
    out << "r_transitive_reflexive_closure" << "(";
    m_role->str(out);
    out << ")";
}

int TransitiveReflexiveClosureRole::compute_evaluate_time_score_impl() const {
    return m_role->compute_evaluate_time_score() + SCORE_QUBIC;
}

}


namespace std {
        bool less<std::shared_ptr<const dlplan::core::TransitiveReflexiveClosureRole>>::operator()(
            const std::shared_ptr<const dlplan::core::TransitiveReflexiveClosureRole>& left_role,
            const std::shared_ptr<const dlplan::core::TransitiveReflexiveClosureRole>& right_role) const {
            return *left_role < *right_role;
        }

        std::size_t hash<dlplan::core::TransitiveReflexiveClosureRole>::operator()(const dlplan::core::TransitiveReflexiveClosureRole& role) const {
            return role.hash();
        }
}
