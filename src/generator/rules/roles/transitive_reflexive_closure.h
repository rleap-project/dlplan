#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class TransitiveReflexiveClosureRole : public Rule {
public:
    TransitiveReflexiveClosureRole() : Rule("r_transitive_reflexive_closure") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        if (iteration == 1) {
            for (const auto& r : data.m_role_iteration_data[iteration].get_elements()) {
            th.submit([&](){
                auto result = data.m_factory->make_transitive_reflexive_closure(r);
                add_role(*this, iteration, std::move(result), states, data);
            });
        }
        }
    }
};

}

#endif
