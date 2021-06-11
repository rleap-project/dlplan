#ifndef DLP_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_
#define DLP_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_

#include "../role.h"
#include "../utils.h"


namespace dlp::core::element {

class TransitiveReflexiveClosureRole : public Role {
protected:
    const Role_Ptr m_role;

protected:
    const RoleDenotation& evaluate_impl(const State& state) override {
        const RoleDenotation& r_vec = m_role->evaluate(state);
        // TODO(dominik): Compute an indexing scheme that only considers objects that are part of the role
        // 2. Compute an adjacency list from the newly mapped role denotations.
        int num_objects = state.get_instance_info()->get_num_objects();
        utils::AdjList adj_list = utils::compute_adjacency_list(r_vec, num_objects);
        // 3. Compute pairwise distances using a sequence of bfs calls.
        utils::PairwiseDistances pairwise_distances = utils::compute_pairwise_distances(adj_list);
        // 4. Extract the transitive closure from the pairwise distances.
        RoleDenotation_Set result_set;
        for (int source = 0; source < static_cast<int>(adj_list.size()); ++source) {
            for (int target : adj_list[source]) {
                if (pairwise_distances[source][target] < INF) {
                    result_set.emplace(source, target);
                }
            }
        }
        // 5. Add the reflexive part
        for (int object_idx = 0; object_idx < num_objects; ++object_idx) {
            result_set.emplace(object_idx, object_idx);
        }
        m_result.clear();
        m_result.insert(m_result.begin(), result_set.begin(), result_set.end());
        return m_result;
    }

public:
    TransitiveReflexiveClosureRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary, "r_transitive_reflexive_closure"), m_role(role) {
        if (!role) {
            throw std::runtime_error("TransitiveReflexiveClosureRole::TransitiveReflexiveClosureRole - child is a nullptr.");
        }
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
