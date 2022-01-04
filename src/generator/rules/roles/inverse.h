#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_INVERSE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_INVERSE_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class InverseRole : public Rule {
public:
    InverseRole() : Rule("r_inverse") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        if (iteration == 1) {
            data.m_role_iteration_data[1].for_each(
                [&](const auto& r){
                    th.submit([&](){
                        auto result = data.m_factory->make_inverse_role(r);
                        add_role(*this, iteration, std::move(result), states, data);
                    });
                }
            );
        }
    }
};

}

#endif
