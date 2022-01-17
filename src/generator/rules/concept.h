#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_CONCEPT_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_CONCEPT_H_

#include "rule.h"


namespace dlplan::generator::rules {

class Concept : public Rule {
protected:
    std::deque<utils::threadpool::ThreadPool::TaskFuture<std::pair<core::Concept,std::array<u_int32_t, 4>>>> m_tasks;

protected:
    virtual void parse_results_of_tasks_impl(int iteration, GeneratorData& data) override {
        /* Wait for the result and add it. */
        while (!m_tasks.empty()) {
            if (data.reached_resource_limit()) return;
            auto result = m_tasks.front().get();
            m_tasks.pop_front();
            if (data.m_concept_hash_table.insert(std::move(result.second))) {
                data.m_reprs.push_back(result.first.compute_repr());
                data.m_concepts_by_iteration[iteration+1].push_back(std::move(result.first));
                ++m_count;
            }
        }
    }

public:
    Concept(const std::string& name) : Rule(name) { }

    virtual void cleanup() override {
        m_tasks.clear();
    }
};

}

#endif
