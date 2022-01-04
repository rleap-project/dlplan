#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_NOT_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_NOT_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class NotRole : public Rule {
public:
    NotRole() : Rule("r_not") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (int i = 1; i < iteration; ++i) {
            data.m_role_iteration_data[i].for_each(
                [&](const auto& r){
                    th.submit([&](){
                        auto result = data.m_factory->make_not_role(r);
                        add_role(*this, iteration, std::move(result), states, data);
                    });
                }
            );
        }
    }
};

}

#endif
