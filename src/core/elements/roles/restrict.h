#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_RESTRICT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_RESTRICT_H_

#include "../role.h"
#include "../concept.h"


namespace dlplan::core::element {

class RestrictRole : public Role {
private:
    RoleDenotation compute_result(RoleDenotation&& role_denot, ConceptDenotation&& concept_denot, RoleDenotation&& result) const {
        for (const auto& pair : role_denot) {
            if (!concept_denot.contains(pair.second)) {
                result.erase(pair);
            }
        }
        return result;
    }

protected:
    const Role_Ptr m_role;
    const Concept_Ptr m_concept;

public:
    RestrictRole(const VocabularyInfo& vocabulary, Role_Ptr role, Concept_Ptr concept)
    : Role(vocabulary), m_role(role), m_concept(concept) {
        if (!(role && concept)) {
            throw std::runtime_error("RestrictRole::RestrictRole - at least one child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        auto role_denot = m_role->evaluate(state);
        auto concept_denot = m_concept->evaluate(state);
        RoleDenotation result = role_denot;
        return compute_result(
            std::move(role_denot),
            std::move(concept_denot),
            std::move(result));
    }

    RoleDenotation evaluate(const State& state, EvaluationCaches& cache) const override {
        if (cache.m_role_denotation_cache.count(state, *this)) {
            return cache.m_role_denotation_cache.find(state, *this);
        }
        auto role_denot = m_role->evaluate(state, cache);
        auto concept_denot = m_concept->evaluate(state, cache);
        auto role_denot_2 = role_denot;
        auto result = compute_result(
            std::move(role_denot),
            std::move(concept_denot),
            std::move(role_denot_2));
        cache.m_role_denotation_cache.insert(state, *this, result);
        return result;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + m_concept->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_role->compute_repr(out);
        out << ",";
        m_concept->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "r_restrict";
    }
};

}

#endif
