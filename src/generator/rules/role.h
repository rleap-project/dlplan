#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_ROLE_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_ROLE_H_

#include "rule.h"


namespace dlplan::generator::rules {

class Role : public Rule {
protected:
    std::deque<utils::threadpool::ThreadPool::TaskFuture<std::pair<core::Role,std::array<u_int32_t, 4>>>> m_tasks;

    static std::function<std::pair<dlplan::core::Role, std::array<uint32_t, 4>>(const States, const core::Role&)> m_task;

protected:
    virtual void parse_results_of_tasks_impl(int iteration, GeneratorData& data) override {
        /* Wait for the result and add it. */
        while (!m_tasks.empty()) {
            if (data.reached_resource_limit()) return;
            auto result = m_tasks.front().get();
            m_tasks.pop_front();
            if (data.m_role_hash_table.insert(std::move(result.second))) {
                data.m_reprs.push_back(result.first.compute_repr());
                data.m_roles_by_iteration[iteration+1].push_back(std::move(result.first));
                ++m_count;
            }
        }
    }

public:
    Role(const std::string& name) : Rule(name) { }

    virtual void cleanup() override {
        m_tasks.clear();
    }
};

inline std::function<std::pair<dlplan::core::Role, std::array<uint32_t, 4>>(const States, const core::Role&)> Role::m_task =
[](const States& states, const core::Role& element) {
    return std::make_pair(std::move(element),compute_hash(bitset_to_num_vec(evaluate<core::RoleDenotation>(element, states))));
};

}

#endif
