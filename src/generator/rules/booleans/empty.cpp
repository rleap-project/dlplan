#include "empty.h"

#include "../../generator_data.h"


namespace dlplan::generator::rules {
void EmptyBoolean::generate_impl(const core::States& states, int target_complexity, dlplan::generator::GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& concept_ : data.m_concepts_by_iteration[target_complexity-1]) {
        auto element = factory.make_empty_boolean(concept_);
        auto denotations = element->evaluate(states, caches);
        if (data.m_boolean_hash_table.insert(denotations).second) {
            std::get<0>(data.m_generated_features).push_back(element);
            data.m_booleans_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }
    for (const auto& role : data.m_roles_by_iteration[target_complexity-1]) {
        auto element = factory.make_empty_boolean(role);
        auto denotations = element->evaluate(states, caches);
        if (data.m_boolean_hash_table.insert(denotations).second) {
            std::get<0>(data.m_generated_features).push_back(element);
            data.m_booleans_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }
}

std::string EmptyBoolean::get_name() const {
    return "b_empty";
}

}
