#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_OR_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_OR_H_

#include "../role.h"


namespace dlplan::core::element {

class OrRole : public Role {
protected:
    Role_Ptr m_role_left;
    Role_Ptr m_role_right;

public:
    OrRole(const VocabularyInfo& vocabulary, Role_Ptr role_1, Role_Ptr role_2)
    : Role(vocabulary, "r_or"),
      m_role_left(role_1),
      m_role_right(role_2) {
        if (!(role_1 && role_2)) {
            throw std::runtime_error("OrRole::OrRole - at least one child is a nullptr.");
        }
        std::stringstream ss1;
        m_role_left->compute_repr(ss1);
        std::stringstream ss2;
        m_role_right->compute_repr(ss2);
        if (ss1.str() > ss2.str()) swap(m_role_left, m_role_right);
    }

    void evaluate(PerElementEvaluationContext& context, RoleDenotation& result) const override {
        const RoleDenotation l = m_role_left->evaluate(context);
        const RoleDenotation r = m_role_right->evaluate(context);
        result.get_data().set(l.get_data());
        result.get_data() |= r.get_data();
    }

    int compute_complexity() const override {
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name << "(";
        m_role_left->compute_repr(out);
        out << ",";
        m_role_right->compute_repr(out);
        out << ")";
    }
};

}

#endif
