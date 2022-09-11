#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PROJECTION_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PROJECTION_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class ProjectionConcept : public Concept {
public:
    ProjectionConcept() : Concept("c_projection") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& r : data.m_roles_by_iteration[iteration]) {
            for (int pos = 0; pos < 2; ++pos) {
                m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_projection_concept(r, pos))));
            }
        }
    }
};

}

#endif
