#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_BOOLEAN_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_BOOLEAN_H_

#include <functional>

#include "rule.h"


namespace dlplan::generator::rules {

class Boolean : public Rule {
protected:
    std::deque<utils::threadpool::ThreadPool::TaskFuture<std::pair<core::Boolean,std::array<u_int32_t, 4>>>> m_tasks;

    inline static std::function<std::pair<dlplan::core::Boolean, std::array<uint32_t, 4>>(const States, const core::Boolean&)> m_task =
        [](const States& states, const core::Boolean& element) {
        return std::make_pair(element, compute_hash(bool_vec_to_num_vec(evaluate<bool>(element, states))));
    };

protected:
    virtual void parse_results_of_tasks_impl(int iteration, GeneratorData& data) override {
        /* Wait for the result and add it. */
        while (!m_tasks.empty()) {
            if (data.reached_resource_limit()) return;
            auto result = m_tasks.front().get();
            m_tasks.pop_front();
            if (data.m_boolean_and_numerical_hash_table.insert(std::move(result.second))) {
                data.m_reprs.push_back(result.first.compute_repr());
                data.m_booleans_by_iteration[iteration+1].push_back(std::move(result.first));
                ++m_count;
            }
        }
    }

public:
    Boolean(const std::string& name) : Rule(name) { }

    virtual void cleanup() override {
        m_tasks.clear();
    }
};

}

#endif
