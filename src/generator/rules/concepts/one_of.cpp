#include "one_of.h"

#include "src/generator/generator_data.h"


namespace dlplan::generator::rules {
void OneOfConcept::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    assert(target_complexity == 1);
    for (const auto& constant : factory.get_vocabulary_info()->get_constants()) {
        auto element = factory.make_one_of_concept(constant);
        auto denotations = element->evaluate(states, caches);
        if (data.m_concept_hash_table.insert(denotations).second) {
            data.m_reprs.push_back(element->compute_repr());
            data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        } else {
            caches.concept_denotations_cache.erase_denotation(element->get_index(), -1, -1);
        }
    }
}

std::string OneOfConcept::get_name() const {
    return "c_one_of";
}

}
