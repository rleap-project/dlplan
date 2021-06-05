#ifndef DLP_SRC_CORE_ELEMENTS_ROLES_NOT_H_
#define DLP_SRC_CORE_ELEMENTS_ROLES_NOT_H_

#include "../role.h"


namespace dlp {
namespace core {
namespace element {

class NotRole : public Role {
protected:
    const Role_Ptr m_role;
    RoleDenotation_Set m_universe_set;

protected:
    virtual const RoleDenotation& evaluate_impl(const State& state) override {
        if (m_universe_set.empty()) {
            unsigned num_objects = state.get_instance_info()->get_num_objects();
            for (unsigned object_idx_1 = 0; object_idx_1 < num_objects; ++object_idx_1) {
                for (unsigned object_idx_2 = 0; object_idx_2 < num_objects; ++object_idx_2) {
                    m_universe_set.emplace(object_idx_1, object_idx_2);
                }
            }
        }
        RoleDenotation_Set r_set = m_universe_set;
        const RoleDenotation& r_vec = m_role->evaluate(state);
        for (const auto& r : r_vec) {
            r_set.erase(r);
        }
        m_result.clear();
        m_result.insert(m_result.begin(), r_set.begin(), r_set.end());
        return m_result;
    }

public:
    NotRole(const VocabularyInfoImpl& vocabulary, Role_Ptr role)
    : Role(vocabulary, "r_not"), m_role(role) {
        if (!role) {
            throw std::runtime_error("NotRole::NotRole - child is a nullptr.");
        }
    }

    virtual unsigned compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role->compute_repr() << ")";
        return ss.str();
    }
};

}
}
}

#endif
