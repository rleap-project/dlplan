#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_NOT_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_NOT_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class NotConcept : public Concept {
public:
    NotConcept() : Concept("c_not") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& c : data.m_concepts_by_iteration[iteration]) {
            m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_not_concept(c))));
        }
    }
};

}

#endif
