#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_CONCEPT_DISTANCE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_CONCEPT_DISTANCE_H_

#include "../numerical.h"
#include "../concept.h"
#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class ConceptDistanceNumerical : public Numerical {
protected:
    const Concept_Ptr m_concept_from;
    const Role_Ptr m_role;
    const Concept_Ptr m_concept_to;

public:
    ConceptDistanceNumerical(const VocabularyInfo& vocabulary, Concept_Ptr concept_from, Role_Ptr role, Concept_Ptr concept_to)
    : Numerical(vocabulary, "n_concept_distance"), m_concept_from(concept_from), m_role(role), m_concept_to(concept_to) {
        if (!(concept_from && role && concept_to)) {
            throw std::runtime_error("ConceptDistanceNumerical::ConceptDistanceNumerical - child is not of type Concept, Role, Concept.");
        }
    }

    int evaluate(const State& state) const override {
        const ConceptDenotation c_from_vec = m_concept_from->evaluate(state);
        if (c_from_vec.empty()) {
            return 0;
        }
        const RoleDenotation r_vec = m_role->evaluate(state);
        const ConceptDenotation c_to_vec = m_concept_to->evaluate(state);
        if (c_to_vec.empty()) {
            return INF;
        }
        // TODO(dominik): Compute an indexing scheme that only considers objects that are part of the role
        // 2. Compute an adjacency list from the newly mapped role denotations.
        int num_objects = state.get_instance_info()->get_num_objects();
        utils::AdjList adj_list = utils::compute_adjacency_list(r_vec, num_objects);
        // 3. Compute pairwise distances using a sequence of bfs calls.
        utils::PairwiseDistances pairwise_distances = utils::compute_pairwise_distances(adj_list);
        // 4. Find closest target.
        int result = INF;
        for (int source : c_from_vec) {
            for (int target : c_to_vec) {
                result = std::min<int>(result, pairwise_distances[source][target]);
            }
        }
        return result;
    }

    int compute_complexity() const override {
        return m_concept_from->compute_complexity() + m_role->compute_complexity() + m_concept_to->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_concept_from->compute_repr() << "," << m_role->compute_repr() << "," << m_concept_to->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif