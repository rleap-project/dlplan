#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_NOT_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_NOT_H_

#include "../concept.h"

#include "../../../core/elements/concepts/not.h"


namespace dlplan::generator::rules {

class NotConcept : public Concept {
public:
    NotConcept() : Concept() { }

    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, core::element::GeneratorEvaluationCaches& caches, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& c : data.m_concepts_by_iteration[target_complexity-1]) {
            m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_not_concept(c)), std::ref(caches)));
        }
    }

    std::string get_name() const override {
        return core::element::NotConcept::get_name();
    }
};

}

#endif
