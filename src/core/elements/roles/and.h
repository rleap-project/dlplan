#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_AND_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_AND_H_

#include "../role.h"


namespace dlplan::core::element {

class AndRole : public Role {
protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

protected:
    const RoleDenotation& evaluate_impl(const State& state) override {
        const RoleDenotation& l_vec = m_role_left->evaluate(state);
        const RoleDenotation& r_vec = m_role_right->evaluate(state);
        RoleDenotation_Set r_set(r_vec.begin(), r_vec.end());
        m_result.clear();
        for (const auto& r : l_vec) {
            if (r_set.find(r) != r_set.end()) {
                m_result.push_back(r);
            }
        }
        return m_result;
    }

public:
    AndRole(const VocabularyInfo& vocabulary, Role_Ptr role_1, Role_Ptr role_2)
    : Role(vocabulary, "r_and"),
      m_role_left(role_1->compute_repr() < role_2->compute_repr() ? role_1 : role_2),
      m_role_right(role_1->compute_repr() < role_2->compute_repr() ? role_2 : role_1) {
        if (!(role_1 && role_2)) {
            throw std::runtime_error("AndRole::AndRole - at least one child is a nullptr.");
        }
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
