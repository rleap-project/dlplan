#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_NOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_NOT_H_

#include "../role.h"


namespace dlplan::core::element {

class NotRole : public Role {
protected:
    const Role_Ptr m_role;

public:
    NotRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary, "r_not"), m_role(role) {
        if (!role) {
            throw std::runtime_error("NotRole::NotRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state, PerElementEvaluationCache& caches, RoleDenotation result) const override {
        RoleDenotation r = m_role->evaluate(state, caches);
        dlplan::utils::BitsetView result_data = result.get_data();
        result_data.set(r.get_data());
        ~result_data;
        return result;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name << "(";
        m_role->compute_repr(out);
        out << ")";
    }
};

}

#endif
