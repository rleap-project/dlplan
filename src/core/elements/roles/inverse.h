#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_INVERSE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_INVERSE_H_

#include "../role.h"


namespace dlplan::core::element {

class InverseRole : public Role {
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
        const auto role_denot = m_role->evaluate(state);
        RoleDenotation result(state.get_instance_info()->get_num_objects());
        for (const auto& pair : role_denot) {
            result.insert(std::make_pair(pair.second, pair.first));
        }
        return result;
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
