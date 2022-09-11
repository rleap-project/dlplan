#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_INCLUSION_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_INCLUSION_H_

#include "../boolean.h"


namespace dlplan::generator::rules {

class InclusionBoolean : public Boolean {
public:
    InclusionBoolean() : Boolean("b_inclusion") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& c1 : data.m_concepts_by_iteration[i]) {
                for (const auto& c2 : data.m_concepts_by_iteration[j]) {
                    m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_concept_inclusion_boolean(c1, c2))));
                }
            }
        }
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& r1 : data.m_roles_by_iteration[i]) {
                for (const auto& r2 : data.m_roles_by_iteration[j]) {
                    m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_role_inclusion_boolean(r1, r2))));
                }
            }
        }
    }
};

}

#endif
