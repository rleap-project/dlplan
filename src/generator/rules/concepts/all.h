#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ALL_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ALL_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class AllConcept : public Concept {
public:
    AllConcept() : Concept("c_all") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory factory = *data.m_factory;
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& r : data.m_roles_by_iteration[i]) {
                for (const auto& c : data.m_concepts_by_iteration[j]) {
                    m_tasks.push_back(th.submit(m_task, std::cref(states), factory.make_all_concept(r, c)));
                }
            }
        }
    }
};

}

#endif
