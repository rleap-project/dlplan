#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_SUM_ROLE_DISTANCE_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_SUM_ROLE_DISTANCE_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class SumRoleDistanceNumerical : public Rule {
public:
    SumRoleDistanceNumerical() : Rule("n_sum_role_distance") { }

    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (int i = 1; i < iteration; ++i) {
            for (int j = 1; j < iteration - i; ++j) {
                int k = iteration - i - j;
                for (const auto& role_left : data.get_role_elements_by_complexity()[i]) {
                    for (const auto& role : data.get_role_elements_by_complexity()[j]) {
                        for (const auto& role_right : data.get_role_elements_by_complexity()[k]) {
                            if (data.reached_limit()) return;
                            else if (data.add_numerical(states, data.get_factory().make_sum_role_distance(role_left, role, role_right))) {
                                m_count_instantiations += 1;
                            }
                        }
                    }
                }
            }
        }
    }
};

}

#endif
