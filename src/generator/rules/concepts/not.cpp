#include "not.h"

#include "../../generator_data.h"

#include "../../../core/elements/concepts/not.h"


namespace dlplan::generator::rules {
void NotConcept::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& c : data.m_concepts_by_iteration[target_complexity-1]) {
        auto element = factory.make_not_concept(c);
        auto denotations = element.get_element()->evaluate(states, caches);
        if (data.m_concept_hash_table.insert(denotations).second) {
            data.m_reprs.push_back(element.compute_repr());
            data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }
}

std::string NotConcept::get_name() const {
    return core::element::NotConcept::get_name();
}

}
