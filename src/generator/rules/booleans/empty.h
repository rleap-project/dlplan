#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_EMPTY_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_EMPTY_H_

#include "../boolean.h"
#include "../../../core/elements/booleans/empty.h"


namespace dlplan::generator::rules {

class EmptyBoolean : public Boolean {
public:
    EmptyBoolean() : Boolean() { }

    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        if (m_lookahead) target_complexity += 1;
        for (const auto& concept : data.m_concepts_by_iteration[target_complexity-1]) {
            m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_empty_boolean(concept))));
        }
        for (const auto& role : data.m_roles_by_iteration[target_complexity-1]) {
            m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_empty_boolean(role))));
        }
    }

    std::string get_name() const override {
        return core::element::EmptyBoolean<int>::get_name();
    }
};

}

#endif
