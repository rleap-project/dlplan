#include "../../../../include/dlplan/core/elements/numericals/role_distance.h"


namespace dlplan::core {
void RoleDistanceNumerical::compute_result(const RoleDenotation& role_from_denot, const RoleDenotation& role_denot, const RoleDenotation& role_to_denot, int& result) const {
    utils::PairwiseDistances pairwise_distances = utils::compute_floyd_warshall(role_denot);
    result = INF;
    int num_objects = role_denot.get_num_objects();
    for (int k = 0; k < num_objects; ++k) {  // property
        for (int i = 0; i < num_objects; ++i) {  // source
            if (role_from_denot.contains(std::make_pair(k, i))) {
                for (int j = 0; j < num_objects; ++j) {  // target
                    if (role_to_denot.contains(std::make_pair(k, j))) {
                        result = std::min<int>(result, pairwise_distances[i][j]);
                    }
                }
            }
        }
    }
}

int RoleDistanceNumerical::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    auto role_from_denot = m_role_from->evaluate(state, caches);
    if (role_from_denot->empty()) {
        return INF;
    }
    auto role_to_denot = m_role_to->evaluate(state, caches);
    if (role_to_denot->empty()) {
        return INF;
    }
    auto role_denot = m_role->evaluate(state, caches);
    int denotation;
    compute_result(*role_from_denot, *role_denot, *role_to_denot, denotation);
    return denotation;
}

NumericalDenotations RoleDistanceNumerical::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    NumericalDenotations denotations;
    denotations.reserve(states.size());
    auto role_from_denots = m_role_from->evaluate(states, caches);
    auto role_denots = m_role->evaluate(states, caches);
    auto role_to_denots = m_role_to->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        if ((*role_from_denots)[i]->empty()) {
            denotations.push_back(INF);
            continue;
        }
        if ((*role_to_denots)[i]->empty()) {
            denotations.push_back(INF);
            continue;
        }
        int denotation;
        compute_result(
            *(*role_from_denots)[i],
            *(*role_denots)[i],
            *(*role_to_denots)[i],
            denotation);
        denotations.push_back(denotation);
    }
    return denotations;
}

RoleDistanceNumerical::RoleDistanceNumerical(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role_from, std::shared_ptr<const Role> role, std::shared_ptr<const Role> role_to)
    : Numerical(index, vocabulary_info, role_from->is_static() && role->is_static() && role_to->is_static()),
        m_role_from(role_from), m_role(role), m_role_to(role_to) { }

bool RoleDistanceNumerical::are_equal_impl(const Numerical& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const RoleDistanceNumerical&>(other);
        return m_is_static == other_derived.m_is_static
            && m_role_from == other_derived.m_role_from
            && m_role == other_derived.m_role
            && m_role_to == other_derived.m_role_to;
    }
    return false;
}

size_t RoleDistanceNumerical::hash_impl() const {
    return hash_combine(m_is_static, m_role_from, m_role, m_role_to);
}

int RoleDistanceNumerical::evaluate(const State& state) const {
    auto role_from_denot = m_role_from->evaluate(state);
    if (role_from_denot.empty()) {
        return INF;
    }
    auto role_to_denot = m_role_to->evaluate(state);
    if (role_to_denot.empty()) {
        return INF;
    }
    auto role_denot = m_role->evaluate(state);
    int denotation;
    compute_result(role_from_denot, role_denot, role_to_denot, denotation);
    return denotation;
}

int RoleDistanceNumerical::compute_complexity_impl() const {
    return m_role_from->compute_complexity() + m_role->compute_complexity() + m_role_to->compute_complexity() + 1;
}

void RoleDistanceNumerical::str_impl(std::stringstream& out) const {
    out << "n_role_distance_numerical" << "(";
    m_role_from->str(out);
    out << ",";
    m_role->str(out);
    out << ",";
    m_role_to->str(out);
    out << ")";
}

int RoleDistanceNumerical::compute_evaluate_time_score_impl() const {
    return m_role_from->compute_evaluate_time_score() + m_role->compute_evaluate_time_score() + m_role_to->compute_evaluate_time_score() + SCORE_QUBIC;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::RoleDistanceNumerical>>::operator()(
        const std::shared_ptr<const dlplan::core::RoleDistanceNumerical>& left_numerical,
        const std::shared_ptr<const dlplan::core::RoleDistanceNumerical>& right_numerical) const {
        return *left_numerical < *right_numerical;
    }

    std::size_t hash<dlplan::core::RoleDistanceNumerical>::operator()(const dlplan::core::RoleDistanceNumerical& numerical) const {
        return numerical.hash();
    }
}
