#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_

#include "../role.h"


namespace dlplan::core::element {

class TopRole : public Role {
public:
    TopRole(const VocabularyInfo& vocabulary)
    : Role(vocabulary, "r_top") {
    }

    RoleDenotation evaluate(const State&, PerElementEvaluationCache&, RoleDenotation result) const override {
        result.get_data().set();
        return result;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name;
    }
};

}

#endif
