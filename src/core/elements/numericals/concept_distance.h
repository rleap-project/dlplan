#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_CONCEPT_DISTANCE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_CONCEPT_DISTANCE_H_

#include "../numerical.h"
#include "../concept.h"
#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class ConceptDistanceNumerical : public Numerical {
private:
    void compute_result(const ConceptDenotation& concept_from_denot, const RoleDenotation& role_denot, const ConceptDenotation& concept_to_denot, int& result) const {
        result = utils::compute_multi_source_multi_target_shortest_distance(concept_from_denot, role_denot, concept_to_denot);
    }

protected:
    const Concept_Ptr m_concept_from;
    const Role_Ptr m_role;
    const Concept_Ptr m_concept_to;

public:
    ConceptDistanceNumerical(const VocabularyInfo& vocabulary, Concept_Ptr concept_from, Role_Ptr role, Concept_Ptr concept_to)
    : Numerical(vocabulary), m_concept_from(concept_from), m_role(role), m_concept_to(concept_to) {
        if (!(concept_from && role && concept_to)) {
            throw std::runtime_error("ConceptDistanceNumerical::ConceptDistanceNumerical - child is not of type Concept, Role, Concept.");
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
        if (concept_from_denot.intersects(concept_to_denot)) {
            return 0;
        }
        auto role_denot = m_role->evaluate(state);
        int denotation;
        compute_result(concept_from_denot, role_denot, concept_to_denot, denotation);
        return denotation;
    }

    const int* evaluate(const State& state, GeneratorEvaluationCaches& cache) const override {
        auto numerical_cache_entry = cache.m_numerical_denotation_cache.find(state, *this);
        assert(numerical_cache_entry != nullptr);
        auto& status = numerical_cache_entry->m_status;
        auto& denotation = numerical_cache_entry->m_denotation;
        if (status) {
            return &denotation;
        }
        auto concept_from_denot = m_concept_from->evaluate(state, cache);
        if (concept_from_denot->empty()) {
            denotation = INF;
            status = true;
            return &denotation;
        }
        auto concept_to_denot = m_concept_to->evaluate(state, cache);
        if (concept_to_denot->empty()) {
            denotation = INF;
            status = true;
            return &denotation;
        }
        if (concept_from_denot->intersects(*concept_to_denot)) {
            denotation = INF;
            status = true;
            return &denotation;
        }
        auto role_denot = m_role->evaluate(state, cache);
        compute_result(
            *concept_from_denot,
            *role_denot,
            *concept_to_denot,
            denotation);
        status = true;
        return &denotation;
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
        return "n_concept_distance";
    }
};

}

#endif
