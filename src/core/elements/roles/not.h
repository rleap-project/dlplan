#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_NOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_NOT_H_

#include "../role.h"


namespace dlplan::core::element {

class NotRole : public Role {
private:
    RoleDenotation compute_result(RoleDenotation&& denot) const {
        return ~denot;
    }

protected:
    const Role_Ptr m_role;

public:
    NotRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary), m_role(role) {
        if (!role) {
            throw std::runtime_error("NotRole::NotRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        return compute_result(m_role->evaluate(state));
    }

    RoleDenotation evaluate(const State& state, EvaluationCaches& cache) const override {
        if (cache.m_role_denotation_cache.count(state, *this)) {
            return cache.m_role_denotation_cache.find(state, *this);
        }
        auto bot_role = RoleDenotation(state.get_instance_info()->get_num_objects());
        auto result = compute_result(
            m_role->evaluate(state, cache));
        cache.m_role_denotation_cache.insert(state, *this, result);
        return result;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_role->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "r_not";
    }
};

}

#endif
