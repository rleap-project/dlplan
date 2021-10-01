#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_

#include "../role.h"


namespace dlplan::core::element {

class TopRole : public Role {
private:
    mutable RoleDenotation m_result;

    mutable int m_num_objects;

public:
    TopRole(const VocabularyInfo& vocabulary)
    : Role(vocabulary, "r_top"), m_num_objects(-1) {
    }

    RoleDenotation evaluate(const State& state) const override {
        return state.get_instance_info()->get_top_role_vec();
    }

    int compute_complexity() const override {
        return 1;
    }

    std::string compute_repr() const override {
        return m_name;
    }
};

}

#endif
