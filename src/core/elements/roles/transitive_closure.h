#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_CLOSURE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_CLOSURE_H_

#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class TransitiveClosureRole : public Role {
protected:
    const Role_Ptr m_role;

public:
    TransitiveClosureRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary, "r_transitive_closure"), m_role(role) {
        if (!role) {
            throw std::runtime_error("TransitiveClosureRole::TransitiveClosureRole - child is a nullptr.");
        }
    }

    const RoleDenotation& evaluate(const State& state) override {
        const RoleDenotation& r_vec = m_role->evaluate(state);
        // TODO(dominik): Compute an indexing scheme that only considers objects that are part of the role
        // 2. Compute an adjacency list from the newly mapped role denotations.
        int num_objects = state.get_instance_info()->get_num_objects();
        utils::AdjList adj_list = utils::compute_adjacency_list(r_vec, num_objects);
        // 3. Compute pairwise distances using a sequence of bfs calls.
        utils::PairwiseDistances pairwise_distances = utils::compute_pairwise_distances(adj_list);
        // 4. Extract the transitive closure from the pairwise distances.
        m_result.clear();
        for (int source = 0; source < static_cast<int>(adj_list.size()); ++source) {
            for (int target : adj_list[source]) {
                if (pairwise_distances[source][target] < INF) {
                    m_result.emplace_back(source, target);
                }
            }
        }
        return m_result;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
