#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_SUM_ROLE_DISTANCE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_SUM_ROLE_DISTANCE_H_

#include "../numerical.h"
#include "../concept.h"
#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class SumRoleDistanceNumerical : public Numerical {
protected:
    const Role_Ptr m_role_from;
    const Role_Ptr m_role;
    const Role_Ptr m_role_to;

public:
    SumRoleDistanceNumerical(const VocabularyInfo& vocabulary, Role_Ptr role_from, Role_Ptr role, Role_Ptr role_to)
    : Numerical(vocabulary, "n_sum_role_distance"), m_role_from(role_from), m_role(role), m_role_to(role_to) {
        if (!(role_from && role && role_to)) {
            throw std::runtime_error("SumRoleDistanceNumerical::SumRoleDistanceNumerical - child is not of type Role, Role, Role.");
        }
    }

    int evaluate(const State& state) const override {
        const RoleDenotation& r = m_role_from->evaluate(state);
        if (r.count() == 0) {
            return 0;
        }
        const RoleDenotation s = m_role->evaluate(state);
        const RoleDenotation t = m_role_to->evaluate(state);
        if (t.count() == 0) {
            return INF;
        }
        int num_objects = state.get_instance_info()->get_num_objects();
        throw std::runtime_error("not implemented");
    }

    int compute_complexity() const override {
        return m_role_from->compute_complexity() + m_role->compute_complexity() + m_role_to->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role_from->compute_repr() << "," << m_role->compute_repr() << "," << m_role_to->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif