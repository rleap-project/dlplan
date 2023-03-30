#include "inclusion.h"

#include "../../generator_data.h"

#include "../../../core/elements/booleans/inclusion.h"


namespace dlplan::generator::rules {
void InclusionBoolean::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (int i = 1; i < target_complexity - 1; ++i) {
        int j = target_complexity - i - 1;
        for (const auto& c1 : data.m_concepts_by_iteration[i]) {
            for (const auto& c2 : data.m_concepts_by_iteration[j]) {
                auto element = factory.make_inclusion_boolean(c1, c2);
                auto denotations = element.get_element()->evaluate(states, caches);
                if (data.m_boolean_and_numerical_hash_table.insert(denotations).second) {
                    data.m_reprs.push_back(element.compute_repr());
                    data.m_booleans_by_iteration[target_complexity].push_back(std::move(element));
                    increment_generated();
                }
            }
        }
    }
    for (int i = 1; i < target_complexity - 1; ++i) {
        int j = target_complexity - i - 1;
        for (const auto& r1 : data.m_roles_by_iteration[i]) {
            for (const auto& r2 : data.m_roles_by_iteration[j]) {
                auto element = factory.make_inclusion_boolean(r1, r2);
                auto denotations = element.get_element()->evaluate(states, caches);
                if (data.m_boolean_and_numerical_hash_table.insert(denotations).second) {
                    data.m_reprs.push_back(element.compute_repr());
                    data.m_booleans_by_iteration[target_complexity].push_back(std::move(element));
                    increment_generated();
                }
            }
        }
    }
}

std::string InclusionBoolean::get_name() const {
    return core::element::InclusionBoolean<int>::get_name();
}

}
