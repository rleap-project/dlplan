#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_SUM_CONCEPT_DISTANCE_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_SUM_CONCEPT_DISTANCE_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class SumConceptDistanceNumerical : public Rule {
private:
    std::vector<std::vector<core::Concept>> m_concepts_by_iteration;
    std::vector<std::vector<core::Role>> m_roles_by_iteration;

public:
    SumConceptDistanceNumerical() : Rule("n_sum_concept_distance") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        // Copy missing data.
        for (int i = static_cast<int>(m_concepts_by_iteration.size()); i < iteration; ++i) {
            m_concepts_by_iteration.push_back(data.m_concept_iteration_data[i].get_elements());
        }
        for (int i = static_cast<int>(m_roles_by_iteration.size()); i < iteration; ++i) {
            m_roles_by_iteration.push_back(data.m_role_iteration_data[i].get_elements());
        }
        // Use 1 thread for the rule.
        th.submit([&](){
            for (int i = 1; i < iteration; ++i) {
                for (int j = 1; j < iteration - i; ++j) {
                    int k = iteration - i - j;
                    for (const auto& c1 : m_concepts_by_iteration[i]) {
                        for (const auto& r : m_roles_by_iteration[j]) {
                            for (const auto& c2 : m_concepts_by_iteration[k]) {
                                auto result = data.m_factory->make_sum_concept_distance(c1, r, c2);
                                add_numerical(*this, iteration, std::move(result), states, data);
                            }
                        }
                    }
                }
            }
        });
    }
};

}

#endif
