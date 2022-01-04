#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_ROLE_DISTANCE_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_ROLE_DISTANCE_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class RoleDistanceNumerical : public Rule {
public:
    RoleDistanceNumerical() : Rule("n_role_distance") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (int i = 1; i < iteration; ++i) {
            for (int j = 1; j < iteration - i; ++j) {
                int k = iteration - i - j;
                for (const auto& r1 : data.m_role_iteration_data[i].get_elements()) {
                    for (const auto& r2 : data.m_role_iteration_data[j].get_elements()) {
                        for (const auto& r3 : data.m_role_iteration_data[k].get_elements()) {
                            th.submit([&](){
                                auto result = data.m_factory->make_role_distance(r1, r2, r3);
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
