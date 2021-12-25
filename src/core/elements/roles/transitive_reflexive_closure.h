#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_

#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class TransitiveReflexiveClosureRole : public Role {
protected:
    const Role_Ptr m_role;

public:
    TransitiveReflexiveClosureRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary, "r_transitive_reflexive_closure"), m_role(role) {
        if (!role) {
            throw std::runtime_error("TransitiveReflexiveClosureRole::TransitiveReflexiveClosureRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation r = m_role->evaluate(state);
        int num_objects = state.get_instance_info()->get_num_objects();
        for (int k = 0; k < num_objects; ++k) {
            for (int i = 0; i < num_objects; ++i) {
                int ik = i * num_objects + k;
                for (int j = 0; j < num_objects; ++j) {
                    int ij = i * num_objects + j;
                    int kj = k * num_objects + j;
                    if (r.test(ik) && r.test(kj)) {
                        r.set(ij);
                    }
                }
            }
            // reflexive
            r.set(k * num_objects + k);
        }
        return r;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
