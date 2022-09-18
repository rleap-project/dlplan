#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PROJECTION_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PROJECTION_H_

#include "../concept.h"

#include "../../../core/elements/concepts/projection.h"


namespace dlplan::generator::rules {

class ProjectionConcept : public Concept {
public:
    ProjectionConcept() : Concept() { }

    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, core::element::GeneratorEvaluationCaches& caches, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& r : data.m_roles_by_iteration[target_complexity-1]) {
            for (int pos = 0; pos < 2; ++pos) {
                m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_projection_concept(r, pos)), std::ref(caches)));
            }
        }
    }

    std::string get_name() const override {
        return core::element::ProjectionConcept::get_name();
    }
};

}

#endif
