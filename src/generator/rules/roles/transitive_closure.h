#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_TRANSITIVE_CLOSURE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_TRANSITIVE_CLOSURE_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class TransitiveClosureRole : public Rule {
public:
    TransitiveClosureRole() : Rule("r_transitive_closure") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        if (iteration == 1) {
            data.m_role_iteration_data[1].for_each(
                [&](const auto& r){
                    th.submit([&](){
                        auto result = data.m_factory->make_transitive_closure(r);
                        add_role(*this, iteration, std::move(result), states, data);
                    });
                }
            );
        }
    }
};

}

#endif
