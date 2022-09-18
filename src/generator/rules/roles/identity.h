#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_IDENTITY_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_IDENTITY_H_

#include "../role.h"

#include "../../../core/elements/roles/identity.h"


namespace dlplan::generator::rules {

class IdentityRole : public Role {
public:
    IdentityRole() : Role() { }

    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, core::element::GeneratorEvaluationCaches& caches, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& c : data.m_concepts_by_iteration[target_complexity-1]) {
            m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_identity_role(c)), std::ref(caches)));
        }
    }

    std::string get_name() const override {
        return core::element::IdentityRole::get_name();
    }
};

}

#endif
