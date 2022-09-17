#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_COUNT_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_COUNT_H_

#include "../numerical.h"

#include "../../../core/elements/numericals/count.h"


namespace dlplan::generator::rules {

class CountNumerical : public Numerical {
public:
    CountNumerical() : Numerical() { }

    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& c : data.m_concepts_by_iteration[target_complexity-1]) {
            m_tasks.push_back(th.submit(m_task, std::cref(states),factory.make_count(c)));
        }
        for (const auto& r : data.m_roles_by_iteration[target_complexity-1]) {
            m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_count(r))));
        }
    }

    std::string get_name() const override {
        return core::element::CountNumerical<int>::get_name();
    }
};

}

#endif
