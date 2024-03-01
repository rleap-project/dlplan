#include "or.h"

#include "../../generator_data.h"


namespace dlplan::generator::rules {
void OrRole::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (int i = 1; i < target_complexity - 1; ++i) {
        int j = target_complexity - i - 1;
        for (const auto& r1 : data.m_roles_by_iteration[i]) {
            for (const auto& r2 : data.m_roles_by_iteration[j]) {
                auto element = factory.make_or_role(r1, r2);
                auto denotations = element->evaluate(states, caches);
                if (data.m_role_hash_table.insert(denotations).second) {
                    std::get<3>(data.m_generated_features).push_back(element);
                    data.m_roles_by_iteration[target_complexity].push_back(std::move(element));
                    increment_generated();
                }
            }
        }
    }
}

std::string OrRole::get_name() const {
    return "r_or";
}

}
