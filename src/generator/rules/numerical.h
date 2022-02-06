#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_NUMERICAL_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_NUMERICAL_H_

#include <functional>

#include "rule.h"


namespace dlplan::generator::rules {

struct NumericalTaskResult {
    dlplan::core::Numerical numerical;
    std::string repr;
    std::array<uint32_t, 4> hash;
};

class Numerical : public Rule {
protected:
    std::deque<utils::threadpool::ThreadPool::TaskFuture<NumericalTaskResult>> m_tasks;

    static std::function<NumericalTaskResult(const States, const core::Numerical&)> m_task;

protected:
    void parse_results_of_tasks_impl(int iteration, GeneratorData& data) override;

public:
    Numerical(const std::string& name) : Rule(name) { }

    void cleanup() override;
};

}

#endif
