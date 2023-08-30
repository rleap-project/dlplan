#include "primitive.h"

#include "../../generator_data.h"

#include "../../../core/parser/expressions/concepts/primitive.h"


namespace dlplan::generator::rules {
void PrimitiveConcept::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    assert(target_complexity == 1);
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& predicate : factory.get_vocabulary_info()->get_predicates()) {
        if (predicate.get_arity() == 1) {
            auto element = factory.make_primitive_concept(predicate, 0);
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
}

const std::string& PrimitiveConcept::get_name() const {
    return core::parser::PrimitiveConcept::get_name();
}

}
