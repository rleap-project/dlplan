#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_SUBSET_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_SUBSET_H_

#include "../concept.h"

#include "../../../core/elements/concepts/subset.h"


namespace dlplan::generator::rules {

class SubsetConcept : public Concept {
public:
    SubsetConcept() : Concept() { }

    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        if (target_complexity == 3) {
            core::SyntacticElementFactory& factory = data.m_factory;
            for (int i = 1; i <= target_complexity - 1; ++i) {
                int j = target_complexity - i - 1;
                for (const auto& r1 : data.m_roles_by_iteration[i]) {
                    for (const auto& r2 : data.m_roles_by_iteration[j]) {
                        m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_subset_concept(r1, r2))));
                    }
                }
            }
        }
    }

    std::string get_name() const override {
        return core::element::SubsetConcept::get_name();
    }
};

}

#endif
