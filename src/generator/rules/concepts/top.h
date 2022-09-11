#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_TOP_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_TOP_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class TopConcept : public Concept {
public:
    TopConcept() : Concept("c_top") { }

    virtual void submit_tasks_impl(const States& states, int, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_top_concept())));
    }
};

}

#endif
