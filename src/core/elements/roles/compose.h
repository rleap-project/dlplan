#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_COMPOSE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_COMPOSE_H_

#include "../role.h"
#include <algorithm>


namespace dlplan::core::element {

class ComposeRole : public Role {
protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

public:
    ComposeRole(const VocabularyInfo& vocabulary, Role_Ptr role_left, Role_Ptr role_right)
    : Role(vocabulary, "r_compose"), m_role_left(role_left), m_role_right(role_right)  {
        if (!(role_left && role_right)) {
            throw std::runtime_error("ComposeRole::ComposeRole - at least one child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        const auto l = m_role_left->evaluate(state);
        const auto& l_data = l.get_const_data();
        const auto r = m_role_right->evaluate(state);
        const auto& r_data = r.get_const_data();
        int num_objects = state.get_instance_info()->get_num_objects();
        RoleDenotation result(num_objects);
        auto& result_data = result.get_data();
        for (int i = 0; i < num_objects; ++i) {  // source
            for (int j = 0; j < num_objects; ++j) {  // target
                int ij = i * num_objects + j;
                for (int k = 0; k < num_objects; ++k) {  // middle
                    int ik = i * num_objects + k;
                    int kj = k * num_objects + j;
                    if (l_data.test(ik) && r_data.test(kj)) {
                        result_data.set(ij);
                        break;
                    }
                }
            }
        }
        return result;
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
