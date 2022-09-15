#include "numerical.h"


namespace dlplan::generator::rules {

std::function<NumericalTaskResult(const States&, const core::Numerical&)> Numerical::m_task =
[](const States& states, const core::Numerical& element) {
    return NumericalTaskResult(
        core::Numerical(element),
        element.compute_complexity(),
        element.compute_repr(),
        compute_hash(evaluate_numerical(element, states)),
        false);
};

void Numerical::parse_results_of_tasks_impl(GeneratorData& data) {
    /* Wait for the result and add it. */
    while (!m_tasks.empty()) {
        if (data.reached_resource_limit()) return;
        auto result = m_tasks.front().get();
        m_tasks.pop_front();
        ++data.m_num_generated_features;
        if (result.prune) {
            continue;
        }
        if (data.m_boolean_and_numerical_hash_table.insert(std::move(result.hash)).second) {
            ++data.m_num_novel_features;
            ++m_count;
            data.m_reprs.push_back(std::move(result.repr));
            data.m_numericals_by_iteration[result.complexity].push_back(std::move(result.numerical));
        }
    }
}

void Numerical::cleanup() {
    m_tasks.clear();
}

}
