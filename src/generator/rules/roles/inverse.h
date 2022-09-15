#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_INVERSE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_INVERSE_H_

#include "../role.h"

#include "../../../core/elements/roles/inverse.h"


namespace dlplan::generator::rules {

class InverseRole : public Role {
public:
    InverseRole() : Role() { }

    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        if (target_complexity == 2) {
            core::SyntacticElementFactory& factory = data.m_factory;
            for (const auto& r : data.m_roles_by_iteration[target_complexity-1]) {
                m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_inverse_role(r))));
            }
        }
    }

    std::string get_name() const override {
        return core::element::InverseRole::get_name();
    }
};

}

#endif
