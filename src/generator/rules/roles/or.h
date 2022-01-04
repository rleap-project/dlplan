#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_OR_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_OR_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class OrRole : public Rule {
public:
    OrRole() : Rule("r_or") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& r1 : data.m_role_iteration_data[i].get_elements()) {
                for (const auto& r2 : data.m_role_iteration_data[j].get_elements()) {
                    th.submit([&](){
                        auto result = data.m_factory->make_or_role(r1, r2);
                        add_role(*this, iteration, std::move(result), states, data);
                    });
                }
            }
        }
    }
};

}

#endif
