#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_SUM_CONCEPT_DISTANCE_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_SUM_CONCEPT_DISTANCE_H_

#include "../numerical.h"

namespace dlplan::generator::rules {

class SumConceptDistanceNumerical : public Numerical {
public:
    SumConceptDistanceNumerical() : Numerical("n_sum_concept_distance") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (int i = 1; i < iteration; ++i) {
            for (int j = 1; j < std::max(2,iteration - i); ++j) {
                int k = iteration - i - j;
                for (const auto& c1 : data.m_concepts_by_iteration[i]) {
                    for (const auto& r : data.m_roles_by_iteration[j]) {
                        for (const auto& c2 : data.m_concepts_by_iteration[k]) {
                            m_tasks.push_back(
                                th.submit([](const States& states, const core::Concept& c1, const core::Role& r, const core::Concept& c2, core::SyntacticElementFactory& factory) {
                                    auto element = factory.make_sum_concept_distance(c1, r, c2);
                                    auto denotation = evaluate<int>(element, states);
                                    auto hash = compute_hash(denotation);
                                    return std::make_pair(std::move(element),std::move(hash));
                                },
                                std::cref(states),
                                std::cref(c1),
                                std::cref(r),
                                std::cref(c2),
                                std::ref(*data.m_factory))
                            );
                        }
                    }
                }
            }
        }
    }
};

}

#endif
