#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_INVERSE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_INVERSE_H_

#include "../role.h"


namespace dlplan::core::element {

class InverseRole : public Role {
private:
    void compute_result(const RoleDenotation& denot, RoleDenotation& result) const {
        for (const auto& pair : denot) {
            result.insert(std::make_pair(pair.second, pair.first));
        }
    }

protected:
    const Role_Ptr m_role;

public:
    InverseRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary), m_role(role) {
        if (!role) {
            throw std::runtime_error("InverseRole::InverseRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation denotation(state.get_instance_info()->get_num_objects());
        compute_result(
            m_role->evaluate(state),
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
            denotation);
        status = true;
        return &denotation;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_role->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "r_inverse";
    }
};

}

#endif
