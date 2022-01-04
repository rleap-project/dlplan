#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_SUM_CONCEPT_DISTANCE_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_SUM_CONCEPT_DISTANCE_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class SumConceptDistanceNumerical : public Rule {
public:
    SumConceptDistanceNumerical() : Rule("n_sum_concept_distance") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (int i = 1; i < iteration; ++i) {
            for (int j = 1; j < iteration - i; ++j) {
                int k = iteration - i - j;
                for (const auto& c1 : data.m_concept_iteration_data[i].get_elements()) {
                    for (const auto& r : data.m_role_iteration_data[j].get_elements()) {
                        for (const auto& c2 : data.m_concept_iteration_data[k].get_elements()) {
                            th.submit([&](){
                                auto result = data.m_factory->make_sum_concept_distance(c1, r, c2);
                                add_numerical(*this, iteration, std::move(result), states, data);
                            });
                        }
                    }
                }
            }
        }
    }
};

}

#endif
