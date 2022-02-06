#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_ROLE_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_ROLE_H_

#include <functional>

#include "rule.h"


namespace dlplan::generator::rules {

struct RoleTaskResult {
    dlplan::core::Role role;
    std::string repr;
    std::array<uint32_t, 4> hash;
};

class Role : public Rule {
protected:
    std::deque<utils::threadpool::ThreadPool::TaskFuture<RoleTaskResult>> m_tasks;

    static std::function<RoleTaskResult(const States, const core::Role&)> m_task;

protected:
    void parse_results_of_tasks_impl(int iteration, GeneratorData& data) override;

public:
    Role(const std::string& name) : Rule(name) { }

    void cleanup() override;
};

}

#endif
