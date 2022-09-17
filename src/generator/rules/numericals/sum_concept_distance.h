#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_SUM_CONCEPT_DISTANCE_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_SUM_CONCEPT_DISTANCE_H_

#include "../numerical.h"

#include "../../../core/elements/numericals/sum_concept_distance.h"


namespace dlplan::generator::rules {

class SumConceptDistanceNumerical : public Numerical {
public:
    SumConceptDistanceNumerical() : Numerical() { }

    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        int j = 3;  // R:C has complexity 3
        for (int i = 1; i < target_complexity - 3 - 1; ++i) {
            int k = target_complexity - i - j - 1;
            for (const auto& c1 : data.m_concepts_by_iteration[i]) {
                for (const auto& r : data.m_roles_by_iteration[j]) {
                    for (const auto& c2 : data.m_concepts_by_iteration[k]) {
                        m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_sum_concept_distance(c1, r, c2))));
                    }
                }
            }
        }
    }

    std::string get_name() const override {
        return core::element::SumConceptDistanceNumerical::get_name();
    }
};

}

#endif
