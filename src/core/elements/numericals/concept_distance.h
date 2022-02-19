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
        const auto concept_from_denot = m_concept_from->evaluate(state);
        if (concept_from_denot.empty()) {
            return INF;
        }
        const auto concept_to_denot = m_concept_to->evaluate(state);
        if (concept_to_denot.empty()) {
            return INF;
        }
        if (concept_from_denot.intersects(concept_to_denot)) {
            return 0;
        }
        const auto role_denot = m_role->evaluate(state);
        return utils::compute_multi_source_multi_target_shortest_distance(concept_from_denot, role_denot, concept_to_denot);
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
