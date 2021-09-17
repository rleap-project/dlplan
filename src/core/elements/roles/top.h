#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_

#include "../role.h"


namespace dlplan::core::element {

class TopRole : public Role {
private:
    mutable RoleDenotation m_result;

    // Store a weak reference to the InstanceInfo of the previously evaluated state to avoid recomputation.
    mutable std::weak_ptr<const InstanceInfo> m_evaluation_instance;

public:
    TopRole(const VocabularyInfo& vocabulary)
    : Role(vocabulary, "r_top"){
    }

    RoleDenotation evaluate(const State& state) const override {
        std::shared_ptr<const InstanceInfo> instance = state.get_instance_info();
        if (instance != m_evaluation_instance.lock()) {
            int num_objects = state.get_instance_info()->get_num_objects();
            m_result.clear();
            m_result.reserve(num_objects * num_objects);
            for (int object_idx_1 = 0; object_idx_1 < num_objects; ++object_idx_1) {
                for (int object_idx_2 = 0; object_idx_2 < num_objects; ++object_idx_2) {
                    m_result.emplace_back(object_idx_1, object_idx_2);
                }
            }
            m_evaluation_instance = instance;
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
