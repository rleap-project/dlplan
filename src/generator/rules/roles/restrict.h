#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_RESTRICT_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_RESTRICT_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class RestrictRole : public Rule {
public:
    RestrictRole() : Rule("r_restrict") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>& tasks) override {
        if (iteration == 2) {
            th.submit([&](){
                for (const auto& r : data.m_role_iteration_data[1].get_elements()) {
                    for (const auto& c : data.m_concept_iteration_data[1].get_elements()) {
                        auto result = data.m_factory->make_restrict_role(r, c);
                        add_role(*this, iteration, std::move(result), states, data);
                    }
                }
            });
        }
    }
};

}

#endif
