#ifndef DLP_SRC_CORE_ELEMENTS_NUMERICAL_SUM_CONCEPT_DISTANCE_H_
#define DLP_SRC_CORE_ELEMENTS_NUMERICAL_SUM_CONCEPT_DISTANCE_H_

#include "../numerical.h"
#include "../concept.h"
#include "../role.h"
#include "../utils.h"


namespace dlp {
namespace core {
namespace element {

class SumConceptDistanceNumerical : public Numerical {
protected:
    const Concept_Ptr m_concept_from;
    const Role_Ptr m_role;
    const Concept_Ptr m_concept_to;

protected:
    virtual const int& evaluate_impl(const State& state) override {
        const ConceptDenotation& c_from_vec = m_concept_from->evaluate(state);
        if (c_from_vec.empty()) {
            m_result = 0;
            return m_result;
        }
        const RoleDenotation& r_vec = m_role->evaluate(state);
        const ConceptDenotation& c_to_vec = m_concept_to->evaluate(state);
        if (c_to_vec.empty()) {
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
        for (int source : c_from_vec) {
            int min_distance = INF;
            for (int target : c_to_vec) {
                min_distance = std::min<int>(min_distance, pairwise_distances[source][target]);
            }
            m_result += utils::path_addition(m_result, min_distance);
        }
        return m_result;
    }

public:
    SumConceptDistanceNumerical(const VocabularyInfo& vocabulary, Concept_Ptr concept_from, Role_Ptr role, Concept_Ptr concept_to)
    : Numerical(vocabulary, "n_sum_concept_distance"), m_concept_from(concept_from), m_role(role), m_concept_to(concept_to) {
        if (!(concept_from && role && concept_to)) {
            throw std::runtime_error("SumConceptDistanceNumerical::SumConceptDistanceNumerical - child is not of type Concept, Role, Concept.");
        }
    }

    virtual unsigned compute_complexity() const override {
        return m_concept_from->compute_complexity() + m_role->compute_complexity() + m_concept_to->compute_complexity() + 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_concept_from->compute_repr() << "," << m_role->compute_repr() << "," << m_concept_to->compute_repr() << ")";
        return ss.str();
    }
};

}
}
}

#endif