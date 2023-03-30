#include "empty.h"

#include "../../generator_data.h"

#include "../../../core/elements/booleans/empty.h"


namespace dlplan::generator::rules {
void EmptyBoolean::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& concept : data.m_concepts_by_iteration[target_complexity-1]) {
        auto element = factory.make_empty_boolean(concept);
        auto denotations = element.get_element()->evaluate(states, caches);
        if (data.m_boolean_and_numerical_hash_table.insert(denotations).second) {
            data.m_reprs.push_back(element.compute_repr());
            data.m_booleans_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }
    for (const auto& role : data.m_roles_by_iteration[target_complexity-1]) {
        auto element = factory.make_empty_boolean(role);
        auto denotations = element.get_element()->evaluate(states, caches);
        if (data.m_boolean_and_numerical_hash_table.insert(denotations).second) {
            data.m_reprs.push_back(element.compute_repr());
            data.m_booleans_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }
}

std::string EmptyBoolean::get_name() const {
    return core::element::EmptyBoolean<int>::get_name();
}

}
