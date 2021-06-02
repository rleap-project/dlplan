#ifndef DLP_SRC_CORE_ELEMENTS_ROLES_AND_H_
#define DLP_SRC_CORE_ELEMENTS_ROLES_AND_H_

#include "../role.h"


namespace dlp {
namespace core {
namespace element {

class AndRole : public Role {
protected:
    Role_Ptr m_role_left;
    Role_Ptr m_role_right;

protected:
    virtual const RoleDenotation& evaluate_impl(const StateImpl& state) override {
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
    AndRole(const VocabularyInfoImpl& vocabulary, Role_Ptr role_1, Role_Ptr role_2)
    : Role(vocabulary, "r_and") {
        if (!(role_1 && role_2)) {
            throw std::runtime_error("AndRole::AndRole - at least one child is a nullptr.");
        }
        // Element is commutative. Hence sort lexicographically.
        if (role_1->compute_repr() < role_2->compute_repr()) {
            m_role_left = role_1;
            m_role_right = role_2;
        } else {
            m_role_left = role_2;
            m_role_right = role_1;
        }
    }

    virtual unsigned compute_complexity() const override {
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role_left->compute_repr() << "," << m_role_right->compute_repr() << ")";
        return ss.str();
    }
};

}
}
}

#endif
