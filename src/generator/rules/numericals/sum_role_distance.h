#include "../rule.h"


namespace dlplan::generator::rules {

class SumRoleDistanceNumerical : public Rule {
protected:
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

public:
    SumRoleDistanceNumerical(bool enabled) : Rule("c_sum_role_distance", enabled) { }
};

}
