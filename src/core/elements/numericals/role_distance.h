#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_ROLE_DISTANCE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_ROLE_DISTANCE_H_


#include "../numerical.h"
#include "../concept.h"
#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class RoleDistanceNumerical : public Numerical {
private:
    void compute_result(const RoleDenotation& role_from_denot, const RoleDenotation& role_denot, const RoleDenotation& role_to_denot, int& result) const {
        utils::PairwiseDistances pairwise_distances = utils::compute_floyd_warshall(role_denot);
        result = INF;
        int num_objects = role_denot.get_num_objects();
        for (int k = 0; k < num_objects; ++k) {  // property
            for (int i = 0; i < num_objects; ++i) {  // source
                if (role_from_denot.contains(std::make_pair(k, i))) {
                    for (int j = 0; j < num_objects; ++j) {  // target
                        if (role_to_denot.contains(std::make_pair(k, j))) {
                            result = std::min<int>(result, pairwise_distances[i][j]);
                        }
                    }
                }
            }
        }
    }

protected:
    const Role_Ptr m_role_from;
    const Role_Ptr m_role;
    const Role_Ptr m_role_to;

public:
    RoleDistanceNumerical(const VocabularyInfo& vocabulary, Role_Ptr role_from, Role_Ptr role, Role_Ptr role_to)
    : Numerical(vocabulary), m_role_from(role_from), m_role(role), m_role_to(role_to) {
        if (!(role_from && role && role_to)) {
            throw std::runtime_error("ConceptDistanceNumerical::ConceptDistanceNumerical - child is not of type Role, Role, Role.");
        }
    }

    int evaluate(const State& state) const override {
        auto role_from_denot = m_role_from->evaluate(state);
        if (role_from_denot.empty()) {
            return INF;
        }
        auto role_to_denot = m_role_to->evaluate(state);
        if (role_to_denot.empty()) {
            return INF;
        }
        auto role_denot = m_role->evaluate(state);
        int denotation;
        compute_result(role_from_denot, role_denot, role_to_denot, denotation);
        return denotation;
    }

    const int* evaluate(const State& state, GeneratorEvaluationCaches& cache) const override {
        auto numerical_cache_entry = cache.m_numerical_denotation_cache.find(state, *this);
        auto& status = numerical_cache_entry->m_status;
        auto& denotation = numerical_cache_entry->m_denotation;
        if (status) {
            return &denotation;
        }
        auto role_from_denot = m_role_from->evaluate(state, cache);
        if (role_from_denot->empty()) {
            denotation = INF;
            status = true;
            return &denotation;
        }
        auto role_to_denot = m_role_to->evaluate(state, cache);
        if (role_to_denot->empty()) {
            denotation = INF;
            status = true;
            return &denotation;
        }
        auto role_denot = m_role->evaluate(state, cache);
        compute_result(
            *role_from_denot,
            *role_denot,
            *role_to_denot,
            denotation);
        status = true;
        return &denotation;
    }

    int compute_complexity() const override {
        return m_role_from->compute_complexity() + m_role->compute_complexity() + m_role_to->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_role_from->compute_repr(out);
        out << ",";
        m_role->compute_repr(out);
        out << ",";
        m_role_to->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "n_role_distance";
    }
};

}

#endif
