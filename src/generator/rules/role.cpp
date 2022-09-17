#include "role.h"


namespace dlplan::generator::rules {

std::function<RoleTaskResult(const States&, const core::Role&)> Role::m_task =
[](const States& states, const core::Role& element) {
    return RoleTaskResult(
        core::Role(element),
        element.compute_complexity(),
        element.compute_repr(),
        compute_hash(evaluate_role(element, states)),
        false);
};

void Role::parse_results_of_tasks_impl(GeneratorData& data) {
    /* Wait for the result and add it. */
    while (!m_tasks.empty()) {
        if (data.reached_resource_limit()) return;
        auto result = m_tasks.front().get();
        m_tasks.pop_front();
        ++data.m_num_generated_features;
        if (result.prune) {
            continue;
        }
        if (data.m_role_hash_table.insert(std::move(result.hash)).second) {
            ++data.m_num_novel_features;
            ++m_count;
            data.m_reprs.push_back(std::move(result.repr));
            data.m_roles_by_iteration[result.complexity].push_back(std::move(result.role));
        }
    }
}

void Role::cleanup() {
    m_tasks.clear();
}

}
