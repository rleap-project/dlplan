#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_AND_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_AND_H_

#include "../role.h"


namespace dlplan::core::element {

class AndRole : public Role {
private:
    RoleDenotation compute_result(RoleDenotation&& left_denot, RoleDenotation&& right_denot) const {
        return left_denot &= right_denot;
    }

protected:
    Role_Ptr m_role_left;
    Role_Ptr m_role_right;

public:
    AndRole(const VocabularyInfo& vocabulary, Role_Ptr role_1, Role_Ptr role_2)
    : Role(vocabulary),
      m_role_left(role_1),
      m_role_right(role_2) {
        if (!(role_1 && role_2)) {
            throw std::runtime_error("AndRole::AndRole - at least one child is a nullptr.");
        }
        std::stringstream ss1;
        m_role_left->compute_repr(ss1);
        std::stringstream ss2;
        m_role_right->compute_repr(ss2);
        if (ss1.str() > ss2.str()) swap(m_role_left, m_role_right);
    }

    RoleDenotation evaluate(const State& state) const override {
        return compute_result(
            m_role_left->evaluate(state),
            m_role_right->evaluate(state));
    }

    RoleDenotation evaluate(const State& state, EvaluationCaches& cache) const override {
        if (cache.m_role_denotation_cache.count(state, *this)) {
            return cache.m_role_denotation_cache.find(state, *this);
        }
        auto result = compute_result(
            m_role_left->evaluate(state, cache),
            m_role_right->evaluate(state, cache));
        cache.m_role_denotation_cache.insert(state, *this, result);
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
        return "r_and";
    }
};

}

#endif
