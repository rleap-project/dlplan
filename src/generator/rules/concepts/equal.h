#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_EQUAL_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_EQUAL_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class EqualConcept : public Concept {
public:
    EqualConcept() : Concept("c_equal") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        if (iteration == 2) {
            core::SyntacticElementFactory factory = *data.m_factory;
            for (int i = 1; i < iteration; ++i) {
                int j = iteration - i;
                for (const auto& r1 : data.m_roles_by_iteration[i]) {
                    for (const auto& r2 : data.m_roles_by_iteration[j]) {
                        m_tasks.push_back(th.submit(m_task, std::cref(states),factory.make_equal_concept(r1, r2)));
                    }
                }
            }
        }
    }
};

}

#endif
