#include "concept.h"


namespace dlplan::generator::rules {

std::function<ConceptTaskResult(const States&, const core::Concept&)> Concept::m_task =
[](const States& states, const core::Concept& element) {
    return ConceptTaskResult(
        core::Concept(element),
        element.compute_complexity(),
        element.compute_repr(),
        compute_hash(evaluate_concept(element, states)),
        false);
};

void Concept::parse_results_of_tasks_impl(GeneratorData& data) {
    /* Wait for the result and add it. */
    while (!m_tasks.empty()) {
        if (data.reached_resource_limit()) return;
        auto result = m_tasks.front().get();
        m_tasks.pop_front();
        ++data.m_num_generated_features;
        if (result.prune) {
            continue;
        }
        if (data.m_concept_hash_table.insert(std::move(result.hash)).second) {
            ++data.m_num_novel_features;
            ++m_count;
            data.m_reprs.push_back(std::move(result.repr));
            data.m_concepts_by_iteration[result.complexity].push_back(std::move(result.concept));
        }
    }
}

void Concept::cleanup() {
    m_tasks.clear();
}

}
