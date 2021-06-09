#ifndef DLP_SRC_CORE_ELEMENTS_ROLES_TOP_H_
#define DLP_SRC_CORE_ELEMENTS_ROLES_TOP_H_

#include "../role.h"


namespace dlp {
namespace core {
namespace element {

class TopRole : public Role {
protected:
    virtual const RoleDenotation& evaluate_impl(const State& state) override {
        if (m_result.empty()) {
            int num_objects = state.get_instance_info()->get_num_objects();
            m_result.reserve(num_objects * num_objects);
            for (int object_idx_1 = 0; object_idx_1 < num_objects; ++object_idx_1) {
                for (int object_idx_2 = 0; object_idx_2 < num_objects; ++object_idx_2) {
                    m_result.emplace_back(object_idx_1, object_idx_2);
                }
            }
        }
        return m_result;
    }

public:
    TopRole(const VocabularyInfo& vocabulary)
    : Role(vocabulary, "r_top"){
    }

    virtual int compute_complexity() const override {
        return 1;
    }

    virtual std::string compute_repr() const override {
        return m_name;
    }
};

}
}
}

#endif
