#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_NUMERICAL_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_NUMERICAL_H_

#include <functional>

#include "rule.h"


namespace dlplan::generator::rules {

class Numerical : public Rule {
protected:
    std::deque<utils::threadpool::ThreadPool::TaskFuture<std::pair<core::Numerical,std::array<u_int32_t, 4>>>> m_tasks;

    static std::function<std::pair<dlplan::core::Numerical, std::array<uint32_t, 4>>(const States, const core::Numerical&)> m_task;

protected:
    virtual void parse_results_of_tasks_impl(int iteration, GeneratorData& data) override {
        /* Wait for the result and add it. */
        while (!m_tasks.empty()) {
            if (data.reached_resource_limit()) return;
            auto result = m_tasks.front().get();
            m_tasks.pop_front();
            if (data.m_boolean_and_numerical_hash_table.insert(std::move(result.second))) {
                data.m_reprs.push_back(result.first.compute_repr());
                data.m_numericals_by_iteration[iteration+1].push_back(std::move(result.first));
                ++m_count;
            }
        }
    }

public:
    Numerical(const std::string& name) : Rule(name) { }

    virtual void cleanup() override {
        m_tasks.clear();
    }
};

}

#endif
