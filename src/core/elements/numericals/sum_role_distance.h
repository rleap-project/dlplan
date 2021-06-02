#ifndef DLP_SRC_CORE_ELEMENTS_NUMERICAL_SUM_ROLE_DISTANCE_H_
#define DLP_SRC_CORE_ELEMENTS_NUMERICAL_SUM_ROLE_DISTANCE_H_

#include "../numerical.h"
#include "../concept.h"
#include "../role.h"
#include "../utils.h"


namespace dlp {
namespace core {
namespace element {

class SumRoleDistanceNumerical : public Numerical {
protected:
    const Role_Ptr m_role_from;
    const Role_Ptr m_role;
    const Role_Ptr m_role_to;

protected:
    virtual const int& evaluate_impl(const StateImpl& state) override {
        const RoleDenotation& r_from_vec = m_role_from->evaluate(state);
        if (r_from_vec.empty()) {
            m_result = 0;
            return m_result;
        }
        const RoleDenotation& r_vec = m_role->evaluate(state);
        const RoleDenotation& r_to_vec = m_role_to->evaluate(state);
        if (r_to_vec.empty()) {
            m_result = INF;
            return m_result;
        }
        // TODO(dominik): Compute an indexing scheme that only considers objects that are part of the role
        // 2. Compute an adjacency list from the newly mapped role denotations.
        int num_objects = state.get_instance_info()->get_num_objects();
        utils::AdjList adj_list = utils::compute_adjacency_list(r_vec, num_objects);
        // 3. Compute pairwise distances using a sequence of bfs calls.
        utils::PairwiseDistances pairwise_distances = utils::compute_pairwise_distances(adj_list);
        // 4. Find closest target.
        m_result = 0;
        for (const auto& r1 : r_from_vec) {
            int min_distance = INF;
            int source = r1.first;
            for (const auto& r2 : r_to_vec) {
                int target = r2.first;
                // the second component of role denotations must agree.
                if (r1.second == r2.second) {
                    min_distance = std::min<int>(min_distance, pairwise_distances[source][target]);
                }
            }
            m_result += utils::path_addition(m_result, min_distance);
        }
        return m_result;
    }

public:
    SumRoleDistanceNumerical(const VocabularyInfoImpl& vocabulary, Role_Ptr role_from, Role_Ptr role, Role_Ptr role_to)
    : Numerical(vocabulary, "n_sum_role_distance"), m_role_from(role_from), m_role(role), m_role_to(role_to) {
        if (!(role_from && role && role_to)) {
            throw std::runtime_error("SumRoleDistanceNumerical::SumRoleDistanceNumerical - child is not of type Role, Role, Role.");
        }
    }

    virtual unsigned compute_complexity() const override {
        return m_role_from->compute_complexity() + m_role->compute_complexity() + m_role_to->compute_complexity() + 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role_from->compute_repr() << "," << m_role->compute_repr() << "," << m_role_to->compute_repr() << ")";
        return ss.str();
    }
};

}
}
}

#endif