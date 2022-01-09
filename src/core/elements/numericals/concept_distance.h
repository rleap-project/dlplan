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
        const utils::AdjList adj_list = utils::compute_adjacency_list(r);
        return utils::compute_multi_source_multi_target_shortest_distance(adj_list, c.to_vector(), d.to_vector());
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