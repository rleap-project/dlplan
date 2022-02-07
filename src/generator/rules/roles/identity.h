#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_IDENTITY_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_IDENTITY_H_

#include "../role.h"

namespace dlplan::generator::rules {

class IdentityRole : public Role {
public:
    IdentityRole() : Role("r_identity") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory factory = *data.m_factory;
        for (const auto& c : data.m_concepts_by_iteration[iteration]) {
            m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_identity_role(c))));
        }
    }
};

}

#endif
