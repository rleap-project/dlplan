#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_BOOLEAN_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_BOOLEAN_H_

#include <functional>

#include "rule.h"


namespace dlplan::generator::rules {

struct BooleanTaskResult {
    dlplan::core::Boolean boolean;
    std::string repr;
    std::array<uint32_t, 4> hash;

    BooleanTaskResult(dlplan::core::Boolean&& _boolean, std::string&& _repr, std::array<uint32_t, 4>&& _hash)
    : boolean(std::move(_boolean)), repr(std::move(_repr)), hash(std::move(_hash)) { }
};

class Boolean : public Rule {
protected:
    std::deque<utils::threadpool::ThreadPool::TaskFuture<BooleanTaskResult>> m_tasks;

    static std::function<BooleanTaskResult(const States&, const core::Boolean&)> m_task;

protected:
    void parse_results_of_tasks_impl(int iteration, GeneratorData& data) override;

public:
    Boolean(const std::string& name) : Rule(name) { }

    void cleanup() override;
};

}

#endif
