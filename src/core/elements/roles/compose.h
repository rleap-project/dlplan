#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_COMPOSE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_COMPOSE_H_

#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class ComposeRole : public Role {
protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

public:
    ComposeRole(const VocabularyInfo& vocabulary, Role_Ptr role_left, Role_Ptr role_right)
    : Role(vocabulary), m_role_left(role_left), m_role_right(role_right)  {
        if (!(role_left && role_right)) {
            throw std::runtime_error("ComposeRole::ComposeRole - at least one child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        int num_objects = state.get_instance_info()->get_num_objects();
        const auto role_left_bitset = element::utils::role_denot_to_bitset(m_role_left->evaluate(state));
        const auto role_right_bitset = element::utils::role_denot_to_bitset(m_role_right->evaluate(state));
        RoleDenotation result(num_objects);
        for (int i = 0; i < num_objects; ++i) {  // source
            for (int j = 0; j < num_objects; ++j) {  // target
                for (int k = 0; k < num_objects; ++k) {  // middle
                    if (role_left_bitset.test(i * num_objects + k) && role_right_bitset.test(k * num_objects + j)) {
                        result.insert(std::make_pair(i, j));
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
        out << get_name() << "(";
        m_role_left->compute_repr(out);
        out << ",";
        m_role_right->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "r_compose";
    }
};

}

#endif
