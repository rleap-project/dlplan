#include "inverse.h"

#include "../../generator_data.h"

#include "../../../core/elements/roles/inverse.h"


namespace dlplan::generator::rules {
void InverseRole::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& r : data.m_roles_by_iteration[target_complexity-1]) {
        auto element = factory.make_inverse_role(r);
        auto denotations = element.get_element()->evaluate(states, caches);
        if (data.m_role_hash_table.insert(denotations).second) {
            data.m_reprs.push_back(element.compute_repr());
            data.m_roles_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }
}

std::string InverseRole::get_name() const {
    return core::element::InverseRole::get_name();
}
}
