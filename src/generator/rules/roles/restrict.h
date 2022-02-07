#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_RESTRICT_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_RESTRICT_H_

#include "../role.h"

namespace dlplan::generator::rules {

class RestrictRole : public Role {
public:
    RestrictRole() : Role("r_restrict") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        if (iteration == 2) {
            core::SyntacticElementFactory factory = *data.m_factory;
            for (int i = 1; i < iteration; ++i) {
                int j = iteration - i;
                for (const auto& r : data.m_roles_by_iteration[i]) {
                    for (const auto& c : data.m_concepts_by_iteration[j]) {
                        m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_restrict_role(r, c))));
                    }
                }
            }
        }
    }
};

}

#endif
