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

    int evaluate(const State& state) const override {
        const ConceptDenotation c = m_concept_from->evaluate(state);
        const auto& c_data = c.get_const_data();
        if (c_data.count() == 0) {
            return 0;
        }
        const ConceptDenotation d = m_concept_to->evaluate(state);
        const auto& d_data = d.get_const_data();
        if (d_data.count() == 0) {
            return INF;
        }
        const RoleDenotation r = m_role->evaluate(state);

        int num_c = c.get_const_data().count();
        int num_d = d.get_const_data().count();
        const ConceptDenotation a = (num_c <= num_d) ? c : d;
        const ConceptDenotation b = (num_c <= num_d) ? d : c;
        const utils::AdjList adj_list = (num_c <= num_d) ? utils::compute_adjacency_list(r) : utils::compute_adjacency_list(r, true);

        int num_objects = state.get_instance_info()->get_num_objects();
        int result = 0;
        for (int i = 0; i < num_objects; ++i) {  // source
            if (a.get_const_data().test(i)) {
                // TODO: stop the BFS as soon as we find a node in c_to_vec?
                utils::Distances distances = utils::compute_distances_from_state(adj_list, i);
                int min_distance = INF;
                for (int j = 0; j < num_objects; ++j) {  // target
                    if (b.get_const_data().test(j)) {
                        min_distance = std::min<int>(min_distance, distances[j]);
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

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_concept_from->compute_repr() << "," << m_role->compute_repr() << "," << m_concept_to->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif