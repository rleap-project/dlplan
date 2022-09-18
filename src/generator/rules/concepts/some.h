#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_SOME_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_SOME_H_

#include "../concept.h"

#include "../../../core/elements/concepts/some.h"


namespace dlplan::generator::rules {

class SomeConcept : public Concept {
public:
    SomeConcept() : Concept() { }

    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, core::element::GeneratorEvaluationCaches& caches, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (int i = 1; i <= target_complexity - 1; ++i) {
            int j = target_complexity - i - 1;
            for (const auto& r : data.m_roles_by_iteration[i]) {
                for (const auto& c : data.m_concepts_by_iteration[j]) {
                    m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_some_concept(r, c)), std::ref(caches)));
                }
            }
        }
    }

    std::string get_name() const override {
        return core::element::SomeConcept::get_name();
    }
};

}

#endif
