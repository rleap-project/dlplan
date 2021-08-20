#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_DIFF_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_DIFF_H_

#include "../role.h"


namespace dlplan::core::element {

class DiffRole : public Role {
protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

public:
    DiffRole(const VocabularyInfo& vocabulary, Role_Ptr role_left, Role_Ptr role_right)
    : Role(vocabulary, "r_diff"), m_role_left(role_left), m_role_right(role_right)  {
        if (!(role_left && role_right)) {
            throw std::runtime_error("DiffRole::DiffRole - at least one child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        const RoleDenotation& l_vec = m_role_left->evaluate(state);
        const RoleDenotation& r_vec = m_role_right->evaluate(state);
        RoleDenotation_Set r_set(l_vec.begin(), l_vec.end());
        for (const auto& r : r_vec) {
            r_set.erase(r);
        }
        return RoleDenotation(r_set.begin(), r_set.end());
    }

    int compute_complexity() const override {
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role_left->compute_repr() << "," << m_role_right->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
