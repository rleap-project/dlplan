#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_NUMERICAL_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_NUMERICAL_H_

#include <functional>

#include "rule.h"


namespace dlplan::generator::rules {

struct NumericalTaskResult {
    dlplan::core::Numerical numerical;
    int complexity;
    std::string repr;
    std::array<uint32_t, 4> hash;
    bool prune;

    NumericalTaskResult(dlplan::core::Numerical&& _numerical, int complexity, std::string&& _repr, std::array<uint32_t, 4>&& _hash, bool prune)
    : numerical(std::move(_numerical)), complexity(complexity), repr(std::move(_repr)), hash(std::move(_hash)), prune(prune) { }
};

class Numerical : public Rule {
protected:
    std::deque<utils::threadpool::ThreadPool::TaskFuture<NumericalTaskResult>> m_tasks;

    static std::function<NumericalTaskResult(const States&, const core::Numerical&)> m_task;

protected:
    void parse_results_of_tasks_impl(GeneratorData& data) override;

public:
    Numerical() : Rule() { }

    void cleanup() override;
};

}

#endif
