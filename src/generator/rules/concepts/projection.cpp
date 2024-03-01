#include "projection.h"

#include "../../generator_data.h"


namespace dlplan::generator::rules {
void ProjectionConcept::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& r : data.m_roles_by_iteration[target_complexity-1]) {
        for (int pos = 0; pos < 2; ++pos) {
            auto element = factory.make_projection_concept(r, pos);
            auto denotations = element->evaluate(states, caches);
            if (data.m_concept_hash_table.insert(denotations).second) {
                std::get<2>(data.m_generated_features).push_back(element);
                data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
    }
}

std::string ProjectionConcept::get_name() const {
    return "c_projection";
}

}
