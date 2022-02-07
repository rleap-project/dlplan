#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_COUNT_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_COUNT_H_

#include "../numerical.h"


namespace dlplan::generator::rules {

class CountNumerical : public Numerical {
public:
    CountNumerical() : Numerical("n_count") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory factory = *data.m_factory;
        for (const auto& c : data.m_concepts_by_iteration[iteration]) {
            m_tasks.push_back(th.submit(m_task, std::cref(states),factory.make_count(c)));
        }
        for (const auto& r : data.m_roles_by_iteration[iteration]) {
            m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_count(r))));
        }
    }
};

}

#endif
