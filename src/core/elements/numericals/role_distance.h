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

    int evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        auto role_from_denot = m_role_from->evaluate(state, caches);
        if (role_from_denot->empty()) {
            return INF;
        }
        auto role_to_denot = m_role_to->evaluate(state, caches);
        if (role_to_denot->empty()) {
            return INF;
        }
        auto role_denot = m_role->evaluate(state, caches);
        int denotation;
        compute_result(*role_from_denot, *role_denot, *role_to_denot, denotation);
        return denotation;
    }

    std::unique_ptr<NumericalDenotations> evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        auto denotations = std::make_unique<NumericalDenotations>();
        denotations->reserve(states.size());
        auto role_from_denots = m_role_from->evaluate(states, caches);
        auto role_denots = m_role->evaluate(states, caches);
        auto role_to_denots = m_role_to->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            if ((*role_from_denots)[i]->empty()) {
                denotations->push_back(INF);
                continue;
            }
            if ((*role_to_denots)[i]->empty()) {
                denotations->push_back(INF);
                continue;
            }
            int denotation;
            compute_result(
                *(*role_from_denots)[i],
                *(*role_denots)[i],
                *(*role_to_denots)[i],
                denotation);
            denotations->push_back(denotation);
        }
        return denotations;
    }

protected:
    const Role_Ptr m_role_from;
    const Role_Ptr m_role;
    const Role_Ptr m_role_to;

public:
    RoleDistanceNumerical(const VocabularyInfo& vocabulary, Role_Ptr role_from, Role_Ptr role, Role_Ptr role_to)
    : Numerical(vocabulary, role_from->is_static() && role->is_static() && role_to->is_static()),
      m_role_from(role_from), m_role(role), m_role_to(role_to) {
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
