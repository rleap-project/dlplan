#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_INVERSE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_INVERSE_H_

#include "../role.h"


namespace dlplan::core::element {

class InverseRole : public Role {
protected:
    const Role_Ptr m_role;

public:
    InverseRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary, "r_inverse"), m_role(role) {
        if (!role) {
            throw std::runtime_error("InverseRole::InverseRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state, PerElementEvaluationCache& caches, RoleDenotation result) const override {
        RoleDenotation r = m_role->evaluate(state, caches);
        dlplan::utils::BitsetView r_data = r.get_data();
        int num_objects = state.get_instance_info()->get_num_objects();
        dlplan::utils::BitsetView result_data = result.get_data();
        result_data.reset();
        for (int i = 0; i < num_objects; ++i) {
            for (int j = 0; j < num_objects; ++j) {
                if (r_data.test(i * num_objects + j)) {
                    result_data.set(j * num_objects + i);
                }
            }
        }
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
