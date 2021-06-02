#ifndef DLP_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_CLOSURE_H_
#define DLP_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_CLOSURE_H_

#include "../role.h"
#include "../utils.h"


namespace dlp {
namespace core {
namespace element {

class TransitiveClosureRole : public Role {
protected:
    Role_Ptr m_role;

protected:
    virtual const RoleDenotation& evaluate_impl(const StateImpl& state) override {
        const RoleDenotation& r_vec = m_role->evaluate(state);
        // 1. Compute an indexing scheme that only considers objects that are part of the role
        int num_objects = state.get_instance_info()->get_num_objects();
        std::pair<RoleDenotation, std::vector<int>> mappings = utils::remap_role_denotation_indices(r_vec, num_objects);
        RoleDenotation r_vec_new = std::move(mappings.first);
        std::vector<int> decode = std::move(mappings.second);
        // 2. Compute an adjacency list from the newly mapped role denotations.
        utils::AdjList adj_list = utils::compute_adjacency_list(r_vec_new, num_objects);
        // 3. Compute pairwise distances using a sequence of bfs calls.
        utils::PairwiseDistances pairwise_distances = utils::compute_pairwise_distances(adj_list);
        // 4. Extract the transitive closure from the pairwise distances.
        m_result.clear();
        for (int source = 0; source < adj_list.size(); ++source) {
            for (int target : adj_list[source]) {
                if (pairwise_distances[source][target] < INF) {
                    m_result.push_back(std::make_pair(decode[source], decode[target]));
                }
            }
        }
        return m_result;
    }

public:
    TransitiveClosureRole(const VocabularyInfoImpl& vocabulary, Role_Ptr role)
    : Role(vocabulary, "r_transitive_closure"), m_role(role) {
        if (!role) {
            throw std::runtime_error("TransitiveClosureRole::TransitiveClosureRole - child is a nullptr.");
        }
    }

    virtual unsigned compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role->compute_repr() << ")";
        return ss.str();
    }
};

}
}
}

#endif
