#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_OR_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_OR_H_

#include "../concept.h"

#include "../../../core/elements/concepts/or.h"


namespace dlplan::generator::rules {

class OrConcept : public Concept {
public:
    OrConcept() : Concept() { }

    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, core::element::GeneratorEvaluationCaches& caches, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (int i = 1; i <= target_complexity - 1; ++i) {
            int j = target_complexity - i - 1;
            for (const auto& c1 : data.m_concepts_by_iteration[i]) {
                for (const auto& c2 : data.m_concepts_by_iteration[j]) {
                    m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_or_concept(c1, c2)), std::ref(caches)));
                }
            }
        }
    }

    std::string get_name() const override {
        return core::element::OrConcept::get_name();
    }
};

}

#endif
