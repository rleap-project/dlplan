#include "concept.h"


namespace dlplan::generator::rules {

std::function<ConceptTaskResult(const States, const core::Concept&)> Concept::m_task =
[](const States& states, const core::Concept& element) {
    return ConceptTaskResult{
        element,
        element.compute_repr(),
        compute_hash(bitset_to_num_vec(evaluate<core::ConceptDenotation>(element, states)))
    };
};

void Concept::parse_results_of_tasks_impl(int iteration, GeneratorData& data) {
    /* Wait for the result and add it. */
    while (!m_tasks.empty()) {
        if (data.reached_resource_limit()) return;
        auto result = m_tasks.front().get();
        m_tasks.pop_front();
        if (data.m_concept_hash_table.insert(std::move(result.hash))) {
            data.m_reprs.push_back(std::move(result.repr));
            data.m_concepts_by_iteration[iteration+1].push_back(std::move(result.concept));
            ++m_count;
        }
    }
}

void Concept::cleanup() {
    m_tasks.clear();
}

}
