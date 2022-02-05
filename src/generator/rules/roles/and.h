#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_AND_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_AND_H_

#include "../role.h"


namespace dlplan::generator::rules {

class AndRole : public Role {
public:
    AndRole() : Role("r_and") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory factory = *data.m_factory;
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& r1 : data.m_roles_by_iteration[i]) {
                for (const auto& r2 : data.m_roles_by_iteration[j]) {
                    m_tasks.push_back(th.submit(m_task, std::cref(states),factory.make_and_role(r1, r2)));
                }
            }
        }
    }
};

}

#endif
