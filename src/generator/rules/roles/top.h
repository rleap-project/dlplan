#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_TOP_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_TOP_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class TopRole : public Rule {
public:
    TopRole() : Rule("r_top") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>& tasks) override {
        tasks.push_back(th.submit([&](){
            auto result = data.m_factory->make_top_role();
            add_role(*this, iteration, std::move(result), states, data);
        }));
    }
};

}

#endif
