#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_EQUAL_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_EQUAL_H_

#include "../concept.h"

#include "../../../core/elements/concepts/equal.h"


namespace dlplan::generator::rules {

class EqualConcept : public Concept {
private:
    // custom task to prune distance features where either
    // 1. left concept evaluates to more than one object, or
    // 2. middle role is not of form R:C.
    static std::function<ConceptTaskResult(const States&, const core::Role&, const core::Concept&)> m_equal_concept_task;

public:
    EqualConcept() : Concept() { }

    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        if (target_complexity == 3) {
            core::SyntacticElementFactory& factory = data.m_factory;
            for (int i = 1; i <= target_complexity - 1; ++i) {
                int j = target_complexity - i - 1;
                for (const auto& r1 : data.m_roles_by_iteration[i]) {
                    for (const auto& r2 : data.m_roles_by_iteration[j]) {
                        m_tasks.push_back(th.submit(std::cref(m_equal_concept_task), std::cref(states), core::Role(r2), std::move(factory.make_equal_concept(r1, r2))));
                    }
                }
            }
        }
    }

    std::string get_name() const override {
        return core::element::EqualConcept::get_name();
    }
};

}

#endif
