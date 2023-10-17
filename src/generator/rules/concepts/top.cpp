#include "top.h"

#include "src/generator/generator_data.h"


namespace dlplan::generator::rules {
void TopConcept::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    assert(target_complexity == 1);
    core::SyntacticElementFactory& factory = data.m_factory;
    auto element = factory.make_top_concept();
    auto denotations = element->evaluate(states, caches);
    if (data.m_concept_hash_table.insert(denotations).second) {
        data.m_reprs.push_back(element->compute_repr());
        data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
        increment_generated();
    } else {
        caches.concept_denotations_cache.erase_denotation(element->get_index(), -1, -1);
    }
}

std::string TopConcept::get_name() const {
    return "c_top";
}

}
