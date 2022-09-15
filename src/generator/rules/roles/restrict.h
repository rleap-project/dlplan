#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_RESTRICT_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_RESTRICT_H_

#include "../role.h"

#include "../../../core/elements/roles/restrict.h"


namespace dlplan::generator::rules {

class RestrictRole : public Role {
public:
    RestrictRole() : Role() { }

    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        if (target_complexity == 3) {
            core::SyntacticElementFactory& factory = data.m_factory;
            for (int i = 1; i <= target_complexity - 1; ++i) {
                int j = target_complexity - i - 1;
                for (const auto& r : data.m_roles_by_iteration[i]) {
                    for (const auto& c : data.m_concepts_by_iteration[j]) {
                        m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_restrict_role(r, c))));
                    }
                }
            }
        }
    }

    std::string get_name() const override {
        return core::element::RestrictRole::get_name();
    }
};

}

#endif
