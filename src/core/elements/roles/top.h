#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_

#include "../utils.h"

#include "../../../../include/dlplan/core.h"

#include <sstream>

using namespace std::string_literals;


namespace dlplan::core {

class TopRole : public Role {
private:
    RoleDenotation evaluate_impl(const State& state, DenotationsCaches&) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        denotation.set();
        return denotation;
    }

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        RoleDenotations denotations;
        denotations.reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
            denotation.set();
            denotations.push_back(caches.get_role_denotation_cache().insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

public:
    TopRole(std::shared_ptr<const VocabularyInfo> vocabulary_info)
    : Role(vocabulary_info, true) {
    }

    RoleDenotation evaluate(const State& state) const override {
        auto denotation = RoleDenotation(state.get_instance_info()->get_objects().size());
        denotation.set();
        return denotation;

    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name();
    }

    int compute_evaluate_time_score() const override {
        return SCORE_CONSTANT;
    }

    static std::string get_name() {
        return "r_top";
    }
};

}

#endif
