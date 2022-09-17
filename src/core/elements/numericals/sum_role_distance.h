#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_SUM_ROLE_DISTANCE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_SUM_ROLE_DISTANCE_H_

#include "../numerical.h"
#include "../concept.h"
#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class SumRoleDistanceNumerical : public Numerical {
private:
    int compute_result(RoleDenotation&& role_from_denot, RoleDenotation&& role_denot, RoleDenotation&& role_to_denot) const {
        utils::PairwiseDistances pairwise_distances = utils::compute_floyd_warshall(role_denot);
        int result = 0;
        int num_objects = role_denot.get_num_objects();
        for (int k = 0; k < num_objects; ++k) {  // property
            for (int i = 0; i < num_objects; ++i) {  // source
                if (role_from_denot.contains(std::make_pair(k, i))) {
                    int min_distance = INF;
                    for (int j = 0; j < num_objects; ++j) {  // target
                        if (role_to_denot.contains(std::make_pair(k, j))) {
                            min_distance = std::min<int>(min_distance, pairwise_distances[i][j]);
                        }
                    }
                    result = utils::path_addition(result, min_distance);
                }
            }
        }
        return result;
    }

protected:
    const Role_Ptr m_role_from;
    const Role_Ptr m_role;
    const Role_Ptr m_role_to;

public:
    SumRoleDistanceNumerical(const VocabularyInfo& vocabulary, Role_Ptr role_from, Role_Ptr role, Role_Ptr role_to)
    : Numerical(vocabulary), m_role_from(role_from), m_role(role), m_role_to(role_to) {
        if (!(role_from && role && role_to)) {
            throw std::runtime_error("SumRoleDistanceNumerical::SumRoleDistanceNumerical - child is not of type Role, Role, Role.");
        }
    }

    int evaluate(const State& state) const override {
        auto role_from_denot = m_role_from->evaluate(state);
        if (role_from_denot.empty()) {
            return INF;
        }
        auto role_to_denot = m_role_to->evaluate(state);
        if (role_to_denot.empty()) {
            return INF;
        }
        auto role_denot = m_role->evaluate(state);
        return compute_result(std::move(role_from_denot), std::move(role_denot), std::move(role_to_denot));
    }

    int evaluate(const State& state, EvaluationCaches& cache) const override {
        auto role_from_denot = m_role_from->evaluate(state, cache);
        if (role_from_denot.empty()) {
            return INF;
        }
        auto role_to_denot = m_role_to->evaluate(state, cache);
        if (role_to_denot.empty()) {
            return INF;
        }
        auto role_denot = m_role->evaluate(state, cache);
        return compute_result(std::move(role_from_denot), std::move(role_denot), std::move(role_to_denot));
    }

    int compute_complexity() const override {
        return m_role_from->compute_complexity() + m_role->compute_complexity() + m_role_to->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_role_from->compute_repr(out);
        out << ",";
        m_role->compute_repr(out);
        out << ",";
        m_role_to->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "n_sum_role_distance";
    }
};

}

#endif
