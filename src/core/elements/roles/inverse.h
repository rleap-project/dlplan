#ifndef DLP_SRC_CORE_ELEMENTS_ROLES_INVERSE_H_
#define DLP_SRC_CORE_ELEMENTS_ROLES_INVERSE_H_

#include "../role.h"


namespace dlp {
namespace core {
namespace element {

class InverseRole : public Role {
protected:
    const Role_Ptr m_role;

protected:
    virtual const RoleDenotation& evaluate_impl(const StateImpl& state) override {
        const RoleDenotation& r_vec = m_role->evaluate(state);
        m_result.clear();
        m_result.reserve(r_vec.size());
        for (const auto& r : r_vec) {
            m_result.emplace_back(r.second, r.first);
        }
        return m_result;
    }

public:
    InverseRole(const VocabularyInfoImpl& vocabulary, Role_Ptr role)
    : Role(vocabulary, "r_inverse"), m_role(role) {
        if (!role) {
            throw std::runtime_error("InverseRole::InverseRole - child is a nullptr.");
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
