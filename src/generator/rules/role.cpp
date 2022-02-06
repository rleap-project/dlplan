#include "role.h"


namespace dlplan::generator::rules {

std::function<RoleTaskResult(const States, const core::Role&)> Role::m_task =
[](const States& states, const core::Role& element) {
    return RoleTaskResult{
        element,
        element.compute_repr(),
        compute_hash(bitset_to_num_vec(evaluate<core::RoleDenotation>(element, states)))
    };
};

void Role::parse_results_of_tasks_impl(int iteration, GeneratorData& data) {
    /* Wait for the result and add it. */
    while (!m_tasks.empty()) {
        if (data.reached_resource_limit()) return;
        auto result = m_tasks.front().get();
        m_tasks.pop_front();
        if (data.m_role_hash_table.insert(std::move(result.hash))) {
            data.m_reprs.push_back(std::move(result.repr));
            data.m_roles_by_iteration[iteration+1].push_back(std::move(result.role));
            ++m_count;
        }
    }
}

void Role::cleanup() {
    m_tasks.clear();
}

}
