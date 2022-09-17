#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_CONCEPT_DISTANCE_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_CONCEPT_DISTANCE_H_

#include "../numerical.h"

#include "../../../core/elements/numericals/concept_distance.h"


namespace dlplan::generator::rules {



class ConceptDistanceNumerical : public Numerical {
private:
    // custom task to prune distance features where either
    // 1. left concept evaluates to more than one object, or
    // 2. middle role is not of form R:C.
    static std::function<NumericalTaskResult(const States&, const core::Concept&, const core::Role&, const core::Numerical&)> m_concept_distance_task;

public:
    ConceptDistanceNumerical() : Numerical() { }

    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        int j = 3;  // R:C has complexity 3
        for (int i = 1; i < target_complexity - j - 1; ++i) {
            int k = target_complexity - i - j - 1;
            for (const auto& c1 : data.m_concepts_by_iteration[i]) {
                for (const auto& r : data.m_roles_by_iteration[j]) {
                    for (const auto& c2 : data.m_concepts_by_iteration[k]) {
                        m_tasks.push_back(th.submit(std::cref(m_concept_distance_task), std::cref(states), core::Concept(c1), core::Role(r), std::move(factory.make_concept_distance(c1, r, c2))));
                    }
                }
            }
        }
        j = 1;  // R has complexity 1
        for (int i = 1; i < target_complexity - j - 1; ++i) {
            int k = target_complexity - i - j - 1;
            for (const auto& c1 : data.m_concepts_by_iteration[i]) {
                for (const auto& r : data.m_roles_by_iteration[j]) {
                    for (const auto& c2 : data.m_concepts_by_iteration[k]) {
                        m_tasks.push_back(th.submit(std::cref(m_concept_distance_task), std::cref(states), core::Concept(c1), core::Role(r), std::move(factory.make_concept_distance(c1, r, c2))));
                    }
                }
            }
        }
    }

    std::string get_name() const override {
        return core::element::ConceptDistanceNumerical::get_name();
    }
};

}

#endif
