#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_CONCEPT_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_CONCEPT_H_

#include <functional>

#include "rule.h"


namespace dlplan::generator::rules {

struct ConceptTaskResult {
    dlplan::core::Concept concept;
    int complexity;
    std::string repr;
    std::array<uint32_t, 4> hash;

    ConceptTaskResult(dlplan::core::Concept&& _concept, int complexity, std::string&& _repr, std::array<uint32_t, 4>&& _hash)
    : concept(std::move(_concept)), complexity(complexity), repr(std::move(_repr)), hash(std::move(_hash)) { }
};

class Concept : public Rule {
protected:
    std::deque<utils::threadpool::ThreadPool::TaskFuture<ConceptTaskResult>> m_tasks;

    static std::function<ConceptTaskResult(const States&, const core::Concept&)> m_task;

protected:
    void parse_results_of_tasks_impl(GeneratorData& data) override;

public:
    Concept() : Rule() { }

    void cleanup() override;
};

}

#endif
