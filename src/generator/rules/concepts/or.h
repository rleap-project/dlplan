#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_OR_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_OR_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class OrConcept : public Concept {
public:
    OrConcept() : Concept("c_or") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory factory = *data.m_factory;
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& c1 : data.m_concepts_by_iteration[i]) {
                for (const auto& c2 : data.m_concepts_by_iteration[j]) {
                    m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_or_concept(c1, c2))));
                }
            }
        }
    }
};

}

#endif
