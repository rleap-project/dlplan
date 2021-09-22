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
        int num_objects = state.get_instance_info()->get_num_objects();
        if (m_num_objects != num_objects) {
            m_result.clear();
            m_result.reserve(num_objects * num_objects);
            for (int object_idx_1 = 0; object_idx_1 < num_objects; ++object_idx_1) {
                for (int object_idx_2 = 0; object_idx_2 < num_objects; ++object_idx_2) {
                    m_result.emplace_back(object_idx_1, object_idx_2);
                }
            }
            m_num_objects = num_objects;
        }
        return m_result;
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
