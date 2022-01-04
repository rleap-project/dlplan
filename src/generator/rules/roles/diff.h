#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_DIFF_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_DIFF_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class DiffRole : public Rule {
public:
    DiffRole() : Rule("r_diff") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            data.m_role_iteration_data[i].for_each(
                [&](const auto& r1){
                    data.m_role_iteration_data[j].for_each(
                        [&](const auto& r2){
                            th.submit([&](){
                                auto result = data.m_factory->make_diff_role(r1, r2);
                                add_role(*this, iteration, std::move(result), states, data);
                            });
                        }
                    );
                }
            );
        }
    }
};

}

#endif
