#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_ROLE_DISTANCE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_ROLE_DISTANCE_H_


#include "../numerical.h"
#include "../concept.h"
#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class RoleDistanceNumerical : public Numerical {
protected:
    const Role_Ptr m_role_from;
    const Role_Ptr m_role;
    const Role_Ptr m_role_to;

public:
    RoleDistanceNumerical(const VocabularyInfo& vocabulary, Role_Ptr role_from, Role_Ptr role, Role_Ptr role_to)
    : Numerical(vocabulary, "n_role_distance"), m_role_from(role_from), m_role(role), m_role_to(role_to) {
        if (!(role_from && role && role_to)) {
            throw std::runtime_error("ConceptDistanceNumerical::ConceptDistanceNumerical - child is not of type Role, Role, Role.");
        }
    }

    int evaluate(const State& state, EvaluationCaches& caches) const override {
        RoleDenotation r = m_role_from->evaluate(state, caches);
        dlplan::utils::BitsetView r_data = r.get_data();
        if (r_data.none()) {
            return INF;
        }
        RoleDenotation t = m_role_to->evaluate(state, caches);
        dlplan::utils::BitsetView t_data = t.get_data();
        if (t_data.none()) {
            return INF;
        }
        RoleDenotation s = m_role->evaluate(state, caches);
        int num_objects = state.get_instance_info()->get_num_objects();
        utils::AdjList adj_list = utils::compute_adjacency_list(s);
        // 3. Compute pairwise distances using a sequence of bfs calls.
        utils::PairwiseDistances pairwise_distances = utils::compute_floyd_warshall(adj_list, true);
        int result = INF;
        for (int k = 0; k < num_objects; ++k) {  // property
            for (int i = 0; i < num_objects; ++i) {  // source
                int ki = k * num_objects + i;
                if (r_data.test(ki)) {
                    for (int j = 0; j < num_objects; ++j) {  // target
                        int kj = k * num_objects + j;
                        if (t_data.test(kj)) {
                            result = std::min<int>(result, pairwise_distances[i][j]);
                        }
                    }
                }
            }
        }
        return result;
    }

    int compute_complexity() const override {
        return m_role_from->compute_complexity() + m_role->compute_complexity() + m_role_to->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name << "(";
        m_role_from->compute_repr(out);
        out << ",";
        m_role->compute_repr(out);
        out << ",";
        m_role_to->compute_repr(out);
        out << ")";
    }
};

}

#endif
