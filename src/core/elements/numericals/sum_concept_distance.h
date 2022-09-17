#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_SUM_CONCEPT_DISTANCE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_SUM_CONCEPT_DISTANCE_H_

#include "../numerical.h"
#include "../concept.h"
#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class SumConceptDistanceNumerical : public Numerical {
private:
    int compute_result(ConceptDenotation&& concept_from_denot, RoleDenotation&& role_denot, ConceptDenotation&& concept_to_denot) const {
        int result = 0;
        utils::Distances source_distances = utils::compute_multi_source_multi_target_shortest_distances(concept_from_denot, role_denot, concept_to_denot);
        for (const auto single : concept_from_denot) {
            result = utils::path_addition(result, source_distances[single]);
        }
        return result;
    }

protected:
    const Concept_Ptr m_concept_from;
    const Role_Ptr m_role;
    const Concept_Ptr m_concept_to;

public:
    SumConceptDistanceNumerical(const VocabularyInfo& vocabulary, Concept_Ptr concept_from, Role_Ptr role, Concept_Ptr concept_to)
    : Numerical(vocabulary), m_concept_from(concept_from), m_role(role), m_concept_to(concept_to) {
        if (!(concept_from && role && concept_to)) {
            throw std::runtime_error("SumConceptDistanceNumerical::SumConceptDistanceNumerical - child is not of type Concept, Role, Concept.");
        }
    }

    int evaluate(const State& state) const override {
        auto concept_from_denot = m_concept_from->evaluate(state);
        if (concept_from_denot.empty()) {
            return INF;
        }
        auto concept_to_denot = m_concept_to->evaluate(state);
        if (concept_to_denot.empty()) {
            return INF;
        }
        auto role_denot = m_role->evaluate(state);
        return compute_result(std::move(concept_from_denot), std::move(role_denot), std::move(concept_to_denot));
    }

    int evaluate(const State& state, EvaluationCaches& cache) const override {
        auto concept_from_denot = m_concept_from->evaluate(state, cache);
        if (concept_from_denot.empty()) {
            return INF;
        }
        auto concept_to_denot = m_concept_to->evaluate(state, cache);
        if (concept_to_denot.empty()) {
            return INF;
        }
        auto role_denot = m_role->evaluate(state, cache);
        return compute_result(std::move(concept_from_denot), std::move(role_denot), std::move(concept_to_denot));
    }

    int compute_complexity() const override {
        return m_concept_from->compute_complexity() + m_role->compute_complexity() + m_concept_to->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_concept_from->compute_repr(out);
        out << ",";
        m_role->compute_repr(out);
        out << ",";
        m_concept_to->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "n_sum_concept_distance";
    }
};

}

#endif
