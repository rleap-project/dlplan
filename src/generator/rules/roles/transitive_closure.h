#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_TRANSITIVE_CLOSURE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_TRANSITIVE_CLOSURE_H_

#include "../role.h"

#include "../../../core/elements/roles/transitive_closure.h"


namespace dlplan::generator::rules {

class TransitiveClosureRole : public Role {
public:
    TransitiveClosureRole() : Role() { }

    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, core::element::GeneratorEvaluationCaches& caches, utils::threadpool::ThreadPool& th) override {
        if (target_complexity == 2) {
            core::SyntacticElementFactory& factory = data.m_factory;
            for (const auto& r : data.m_roles_by_iteration[target_complexity-1]) {
                m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_transitive_closure(r)), std::ref(caches)));
            }
        }
    }

    std::string get_name() const override {
        return core::element::TransitiveClosureRole::get_name();
    }
};

}

#endif
