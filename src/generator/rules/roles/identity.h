#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_IDENTITY_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_IDENTITY_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class IdentityRole : public Rule {
public:
    IdentityRole() : Rule("r_identity") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (const auto& c : data.m_concept_iteration_data[iteration].get_elements()) {
            th.submit([&](){
                auto result = data.m_factory->make_identity_role(c);
                add_role(*this, iteration, std::move(result), states, data);
            });
        }
    }
};

}

#endif
