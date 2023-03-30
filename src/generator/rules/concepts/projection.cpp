#include "projection.h"

#include "../../generator_data.h"

#include "../../../core/elements/concepts/projection.h"


namespace dlplan::generator::rules {
void ProjectionConcept::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& r : data.m_roles_by_iteration[target_complexity-1]) {
        for (int pos = 0; pos < 2; ++pos) {
            auto element = factory.make_projection_concept(r, pos);
            auto denotations = element.get_element()->evaluate(states, caches);
            if (data.m_concept_hash_table.insert(denotations).second) {
                data.m_reprs.push_back(element.compute_repr());
                data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
    }
}

std::string ProjectionConcept::get_name() const {
    return core::element::ProjectionConcept::get_name();
}

}
