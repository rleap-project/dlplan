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
        const ConceptDenotation c = m_concept_from->evaluate(state);
        if (c.count() == 0) {
            return 0;
        }
        const RoleDenotation r = m_role->evaluate(state);
        const ConceptDenotation d = m_concept_to->evaluate(state);
        if (d.count() == 0) {
            return INF;
        }

        int num_objects = state.get_instance_info()->get_num_objects();
        utils::AdjList adj_list = utils::compute_adjacency_list(r, num_objects);
        int result = INF;
        for (int i = 0; i < num_objects; ++i) {
            if (c.test(i)) {
                // TODO: stop the BFS as soon as we find a node in c_to_vec?
                utils::Distances distances = utils::compute_distances_from_state(adj_list, i);
                for (int j = 0; j < num_objects; ++j) {
                    if (d.test(j)) {
                        result = std::min<int>(result, distances[j]);
                    }
                }
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