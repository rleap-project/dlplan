#include "count.h"

#include "../../generator_data.h"


namespace dlplan::generator::rules {
void CountNumerical::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& concept_ : data.m_concepts_by_iteration[target_complexity-1]) {
        auto element = factory.make_count_numerical(concept_);
        auto denotations = element->evaluate(states, caches);
        if (data.m_numerical_hash_table.insert(denotations).second) {
            std::get<1>(data.m_generated_features).push_back(element);
            data.m_numericals_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }
    for (const auto& role : data.m_roles_by_iteration[target_complexity-1]) {
        auto element = factory.make_count_numerical(role);
        auto denotations = element->evaluate(states, caches);
        if (data.m_numerical_hash_table.insert(denotations).second) {
            std::get<1>(data.m_generated_features).push_back(element);
            data.m_numericals_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }
}

std::string CountNumerical::get_name() const {
    return "n_count";
}

}
