#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_SUM_CONCEPT_DISTANCE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_SUM_CONCEPT_DISTANCE_H_

#include "../utils.h"

#include "../../../../include/dlplan/core.h"

#include <sstream>

using namespace std::string_literals;


namespace dlplan::core {

class SumConceptDistanceNumerical : public Numerical {
private:
    void compute_result(const ConceptDenotation& concept_from_denot, const RoleDenotation& role_denot, const ConceptDenotation& concept_to_denot, int& result) const {
        result = 0;
        utils::Distances source_distances = utils::compute_multi_source_multi_target_shortest_distances(concept_from_denot, role_denot, concept_to_denot);
        for (const auto target : concept_to_denot.to_vector()) {
            result = utils::path_addition(result, source_distances[target]);
        }
    }

    int evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        auto concept_from_denot = m_concept_from->evaluate(state, caches);
        if (concept_from_denot->empty()) {
            return INF;
        }
        auto concept_to_denot = m_concept_to->evaluate(state, caches);
        if (concept_to_denot->empty()) {
            return INF;
        }
        if (concept_from_denot->intersects(*concept_to_denot)) {
            return 0;
        }
        auto role_denot = m_role->evaluate(state, caches);
        int denotation;
        compute_result(
            *concept_from_denot,
            *role_denot,
            *concept_to_denot, denotation);
        return denotation;
    }

    NumericalDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        NumericalDenotations denotations;
        denotations.reserve(states.size());
        auto concept_from_denots = m_concept_from->evaluate(states, caches);
        auto role_denots = m_role->evaluate(states, caches);
        auto concept_to_denots = m_concept_to->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            if ((*concept_from_denots)[i]->empty()) {
                denotations.push_back(INF);
                continue;
            }
            if ((*concept_to_denots)[i]->empty()) {
                denotations.push_back(INF);
                continue;
            }
            int denotation;
            compute_result(
                *(*concept_from_denots)[i],
                *(*role_denots)[i],
                *(*concept_to_denots)[i],
                denotation);
            denotations.push_back(denotation);
        }
        return denotations;
    }

protected:
    const std::shared_ptr<const Concept> m_concept_from;
    const std::shared_ptr<const Role> m_role;
    const std::shared_ptr<const Concept> m_concept_to;

public:
    SumConceptDistanceNumerical(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const Concept> concept_from, std::shared_ptr<const Role> role, std::shared_ptr<const Concept> concept_to)
    : Numerical(vocabulary_info, concept_from->is_static() && role->is_static() && concept_to->is_static()),
      m_concept_from(concept_from), m_role(role), m_concept_to(concept_to) {
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
        int denotation;
        compute_result(concept_from_denot, role_denot, concept_to_denot, denotation);
        return denotation;
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

    int compute_evaluate_time_score() const override {
        return m_concept_from->compute_evaluate_time_score() + m_role->compute_evaluate_time_score() + m_concept_to->compute_evaluate_time_score() + SCORE_QUBIC;
    }

    static std::string get_name() {
        return "n_sum_concept_distance";
    }
};

}

#endif
