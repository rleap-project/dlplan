#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_AND_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_AND_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class AndConcept : public Concept {
public:
    AndConcept() : Concept("c_and") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory factory = *data.m_factory;
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& c1 : data.m_concepts_by_iteration[i]) {
                for (const auto& c2 : data.m_concepts_by_iteration[j]) {
                    m_tasks.push_back(th.submit(m_task, std::cref(states), factory.make_and_concept(c1, c2)));
                }
            }
        }
    }
};

}

#endif
