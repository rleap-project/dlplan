#include "some.h"

#include "../../generator_data.h"

#include "../../../core/elements/concepts/some.h"


namespace dlplan::generator::rules {
void SomeConcept::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (int i = 1; i < target_complexity - 1; ++i) {
        int j = target_complexity - i - 1;
        for (const auto& r : data.m_roles_by_iteration[i]) {
            for (const auto& c : data.m_concepts_by_iteration[j]) {
                auto element = factory.make_some_concept(r, c);
                auto denotations = element.get_element()->evaluate(states, caches);
                if (data.m_concept_hash_table.insert(denotations).second) {
                    data.m_reprs.push_back(element.compute_repr());
                    data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
                    increment_generated();
                }
            }
        }
    }
}

std::string SomeConcept::get_name() const {
    return core::element::SomeConcept::get_name();
}

}
