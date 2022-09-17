#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_

#include "../role.h"


namespace dlplan::core::element {

class TopRole : public Role {
public:
    TopRole(const VocabularyInfo& vocabulary)
    : Role(vocabulary) {
    }

    RoleDenotation evaluate(const State& state) const override {
        return state.get_instance_info()->get_top_role();
    }

    RoleDenotation evaluate(const State& state, EvaluationCaches&) const override {
        return evaluate(state);
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name();
    }

    static std::string get_name() {
        return "r_top";
    }
};

}

#endif
