#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_RESTRICT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_RESTRICT_H_

#include "../role.h"
#include "../concept.h"


namespace dlplan::core::element {

class RestrictRole : public Role {
private:
    void compute_result(const RoleDenotation& role_denot, const ConceptDenotation& concept_denot, RoleDenotation& result) const {
        result = role_denot;
        for (const auto& pair : role_denot) {
            if (!concept_denot.contains(pair.second)) {
                result.erase(pair);
            }
        }
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
        RoleDenotation denotation(state.get_instance_info()->get_num_objects());
        compute_result(
            m_role->evaluate(state),
            m_concept->evaluate(state),
            denotation);
        return denotation;
    }

    const RoleDenotation* evaluate(const State& state, GeneratorEvaluationCaches& cache) const override {
        auto role_cache_entry = cache.m_role_denotation_cache.find(state, *this);
        auto& status = role_cache_entry->m_status;
        auto& denotation = role_cache_entry->m_denotation;
        if (status) {
            return &denotation;
        }
        compute_result(
            *m_role->evaluate(state, cache),
            *m_concept->evaluate(state, cache),
            denotation);
        status = true;
        return &denotation;
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
