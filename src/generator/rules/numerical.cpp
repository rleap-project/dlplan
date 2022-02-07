#include "numerical.h"


namespace dlplan::generator::rules {

std::function<NumericalTaskResult(const States&, const core::Numerical&)> Numerical::m_task =
[](const States& states, const core::Numerical& element) {
    return NumericalTaskResult(
        core::Numerical(element),
        element.compute_repr(),
        compute_hash(evaluate<int>(element, states)));
};

void Numerical::parse_results_of_tasks_impl(int iteration, GeneratorData& data) {
    /* Wait for the result and add it. */
    while (!m_tasks.empty()) {
        if (data.reached_resource_limit()) return;
        auto result = m_tasks.front().get();
        m_tasks.pop_front();
        if (data.m_boolean_and_numerical_hash_table.insert(std::move(result.hash))) {
            data.m_reprs.push_back(std::move(result.repr));
            data.m_numericals_by_iteration[iteration+1].push_back(std::move(result.numerical));
            ++m_count;
        }
    }
}

void Numerical::cleanup() {
    m_tasks.clear();
}

}
