#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_NOT_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_NOT_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class NotRole : public Rule {
public:
    NotRole() : Rule("r_not") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>& tasks) override {
        tasks.push_back(th.submit([&](){
            for (const auto& r : data.m_role_iteration_data[iteration].get_elements()) {
                add_role(*this, iteration, data.m_factory->make_not_role(r), states, data);
            }
        }));
    }
};

}

#endif
