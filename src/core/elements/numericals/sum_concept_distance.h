#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_SUM_CONCEPT_DISTANCE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_SUM_CONCEPT_DISTANCE_H_

#include "../numerical.h"
#include "../concept.h"
#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class SumConceptDistanceNumerical : public Numerical {
protected:
    const Concept_Ptr m_concept_from;
    const Role_Ptr m_role;
    const Concept_Ptr m_concept_to;

public:
    SumConceptDistanceNumerical(const VocabularyInfo& vocabulary, Concept_Ptr concept_from, Role_Ptr role, Concept_Ptr concept_to)
    : Numerical(vocabulary, "n_sum_concept_distance"), m_concept_from(concept_from), m_role(role), m_concept_to(concept_to) {
        if (!(concept_from && role && concept_to)) {
            throw std::runtime_error("SumConceptDistanceNumerical::SumConceptDistanceNumerical - child is not of type Concept, Role, Concept.");
        }
    }

    int evaluate(const State& state, EvaluationCaches& caches) const override {
        ConceptDenotation c = m_concept_from->evaluate(state, caches);
        dlplan::utils::BitsetView c_data = c.get_data();
        if (c_data.none()) {
            return INF;
        }
        ConceptDenotation d = m_concept_to->evaluate(state, caches);
        dlplan::utils::BitsetView d_data = d.get_data();
        if (d_data.none()) {
            return INF;
        }
        if (c_data.intersects(d_data)) {
            return 0;
        }
        RoleDenotation r = m_role->evaluate(state, caches);
        int num_objects = state.get_instance_info()->get_num_objects();
        utils::AdjList adj_list = utils::compute_adjacency_list(r);
        utils::PairwiseDistances pairwise_distances = utils::compute_floyd_warshall(adj_list, true);
        int result = 0;
        for (int i = 0; i < num_objects; ++i) {  // source
            if (c_data.test(i)) {
                int min_distance = INF;
                for (int j = 0; j < num_objects; ++j) {  // target
                    if (d_data.test(j)) {
                        min_distance = std::min<int>(min_distance, pairwise_distances[i][j]);
                    }
                }
                result = utils::path_addition(result, min_distance);
            }
        }
        return result;
    }

    int compute_complexity() const override {
        return m_concept_from->compute_complexity() + m_role->compute_complexity() + m_concept_to->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name << "(";
        m_concept_from->compute_repr(out);
        out << ",";
        m_role->compute_repr(out);
        out << ",";
        m_concept_to->compute_repr(out);
        out << ")";
    }
};

}

#endif
