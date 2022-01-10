#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_SUM_ROLE_DISTANCE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_SUM_ROLE_DISTANCE_H_

#include "../numerical.h"
#include "../concept.h"
#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class SumRoleDistanceNumerical : public Numerical {
protected:
    const Role_Ptr m_role_from;
    const Role_Ptr m_role;
    const Role_Ptr m_role_to;

public:
    SumRoleDistanceNumerical(const VocabularyInfo& vocabulary, Role_Ptr role_from, Role_Ptr role, Role_Ptr role_to)
    : Numerical(vocabulary, "n_sum_role_distance"), m_role_from(role_from), m_role(role), m_role_to(role_to) {
        if (!(role_from && role && role_to)) {
            throw std::runtime_error("SumRoleDistanceNumerical::SumRoleDistanceNumerical - child is not of type Role, Role, Role.");
        }
    }

    int evaluate(const State& state) const override {
        const RoleDenotation& r = m_role_from->evaluate(state);
        const auto& r_data = r.get_const_data();
        if (r_data.empty()) {
            return 0;
        }
        const RoleDenotation t = m_role_to->evaluate(state);
        const auto& t_data = t.get_const_data();
        if (t_data.empty()) {
            return INF;
        }
        const RoleDenotation s = m_role->evaluate(state);
        int num_objects = state.get_instance_info()->get_num_objects();
        utils::AdjList adj_list = utils::compute_adjacency_list(s);
        // 3. Compute pairwise distances using a sequence of bfs calls.
        utils::PairwiseDistances pairwise_distances = utils::compute_floyd_warshall(adj_list, true);
        int result = 0;
        for (int k = 0; k < num_objects; ++k) {  // property
            for (int i = 0; i < num_objects; ++i) {  // source
                int ki = k * num_objects + i;
                if (r_data.test(ki)) {
                    int min_distance = INF;
                    for (int j = 0; j < num_objects; ++j) {  // target
                        int kj = k * num_objects + j;
                        if (t_data.test(kj)) {
                            min_distance = std::min<int>(min_distance, pairwise_distances[i][j]);
                        }
                    }
                    result = utils::path_addition(result, min_distance);
                }
            }
        }
        return result;
    }

    int compute_complexity() const override {
        return m_role_from->compute_complexity() + m_role->compute_complexity() + m_role_to->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role_from->compute_repr() << "," << m_role->compute_repr() << "," << m_role_to->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
