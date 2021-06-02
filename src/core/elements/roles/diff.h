#ifndef DLP_SRC_CORE_ELEMENTS_ROLES_DIFF_H_
#define DLP_SRC_CORE_ELEMENTS_ROLES_DIFF_H_

#include "../role.h"


namespace dlp {
namespace core {
namespace element {

class DiffRole : public Role {
protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

protected:
    virtual const RoleDenotation& evaluate_impl(const StateImpl& state) override {
        const RoleDenotation& l_vec = m_role_left->evaluate(state);
        const RoleDenotation& r_vec = m_role_right->evaluate(state);
        RoleDenotation_Set r_set(l_vec.begin(), l_vec.end());
        m_result.clear();
        for (const auto& r : r_vec) {
            r_set.erase(r);
        }
        m_result.insert(m_result.begin(), r_set.begin(), r_set.end());
        return m_result;
    }

public:
    DiffRole(const VocabularyInfoImpl& vocabulary, Role_Ptr role_left, Role_Ptr role_right)
    : Role(vocabulary, "r_diff"), m_role_left(role_left), m_role_right(role_right)  {
        if (!(role_left && role_right)) {
            throw std::runtime_error("DiffRole::DiffRole - at least one child is a nullptr.");
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
