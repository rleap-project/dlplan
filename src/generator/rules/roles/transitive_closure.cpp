#include "transitive_closure.h"

#include "../../generator_data.h"

#include "../../../core/elements/roles/transitive_closure.h"


namespace dlplan::generator::rules {
void TransitiveClosureRole::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    if (target_complexity == 2) {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& r : data.m_roles_by_iteration[target_complexity-1]) {
            auto element = factory.make_transitive_closure(r);
            auto denotations = element.get_element()->evaluate(states, caches);
            if (data.m_role_hash_table.insert(denotations).second) {
                data.m_reprs.push_back(element.compute_repr());
                data.m_roles_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
    }
}

std::string TransitiveClosureRole::get_name() const {
    return core::element::TransitiveClosureRole::get_name();
}

}
