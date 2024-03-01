#include "transitive_reflexive_closure.h"

#include "../../generator_data.h"


namespace dlplan::generator::rules {
void TransitiveReflexiveClosureRole::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    if (target_complexity == 2) {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& r : data.m_roles_by_iteration[target_complexity-1]) {
            auto element = factory.make_transitive_reflexive_closure(r);
            auto denotations = element->evaluate(states, caches);
            if (data.m_role_hash_table.insert(denotations).second) {
                std::get<3>(data.m_generated_features).push_back(element);
                data.m_roles_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
    }
}

std::string TransitiveReflexiveClosureRole::get_name() const {
    return "r_transitive_reflexive_closure";
}

}
