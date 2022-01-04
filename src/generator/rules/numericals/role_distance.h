#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_ROLE_DISTANCE_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_ROLE_DISTANCE_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class RoleDistanceNumerical : public Rule {
private:
    std::vector<std::vector<core::Role>> m_roles_by_iteration;

public:
    RoleDistanceNumerical() : Rule("n_role_distance") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        // Copy missing data.
        for (int i = static_cast<int>(m_roles_by_iteration.size()); i < iteration; ++i) {
            m_roles_by_iteration.push_back(data.m_role_iteration_data[i].get_elements());
        }
        // Use 1 thread for the rule.
        th.submit([&](){
            for (int i = 1; i < iteration; ++i) {
                for (int j = 1; j < iteration - i; ++j) {
                    int k = iteration - i - j;
                    for (const auto& r1 : m_roles_by_iteration[i]) {
                        for (const auto& r2 : m_roles_by_iteration[j]) {
                            for (const auto& r3 : m_roles_by_iteration[k]) {
                                auto result = data.m_factory->make_role_distance(r1, r2, r3);
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
