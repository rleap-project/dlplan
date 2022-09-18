#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_

#include "../role.h"
#include "../concept.h"


namespace dlplan::core::element {

class IdentityRole : public Role {
private:
    void compute_result(const ConceptDenotation& denot, RoleDenotation& result) const {
        for (const auto& single : denot) {
            result.insert(std::make_pair(single, single));
        }
    }

protected:
    const Concept_Ptr m_concept;

public:
    IdentityRole(const VocabularyInfo& vocabulary, Concept_Ptr concept)
    : Role(vocabulary), m_concept(concept) {
        if (!concept) {
            throw std::runtime_error("IdentityRole::IdentityRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation denotation(state.get_instance_info()->get_num_objects());
        compute_result(
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
            *m_concept->evaluate(state, cache),
            denotation);
        status = true;
        return &denotation;
    }

    int compute_complexity() const override {
        return m_concept->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_concept->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "r_identity";
    }
};

}

#endif
