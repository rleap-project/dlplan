#include "identity.h"

#include "../../generator_data.h"


namespace dlplan::generator::rules {
void IdentityRole::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& c : data.m_concepts_by_iteration[target_complexity-1]) {
        auto element = factory.make_identity_role(c);
        auto denotations = element->evaluate(states, caches);
        if (data.m_role_hash_table.insert(denotations).second) {
            std::get<3>(data.m_generated_features).push_back(element);
            data.m_roles_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }
}

std::string IdentityRole::get_name() const {
    return "r_identity";
}

}
