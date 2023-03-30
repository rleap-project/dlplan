#include "count.h"

#include "../../generator_data.h"

#include "../../../core/elements/numericals/count.h"


namespace dlplan::generator::rules {
void CountNumerical::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& concept : data.m_concepts_by_iteration[target_complexity-1]) {
        auto element = factory.make_count_numerical(concept);
        auto denotations = element.get_element()->evaluate(states, caches);
        if (data.m_boolean_and_numerical_hash_table.insert(denotations).second) {
            data.m_reprs.push_back(element.compute_repr());
            data.m_numericals_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }
    for (const auto& role : data.m_roles_by_iteration[target_complexity-1]) {
        auto element = factory.make_count_numerical(role);
        auto denotations = element.get_element()->evaluate(states, caches);
        if (data.m_boolean_and_numerical_hash_table.insert(denotations).second) {
            data.m_reprs.push_back(element.compute_repr());
            data.m_numericals_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }
}

std::string CountNumerical::get_name() const {
    return core::element::CountNumerical<int>::get_name();
}

}
