#include "primitive.h"

#include "../../generator_data.h"

#include "../../../core/elements/concepts/primitive.h"


namespace dlplan::generator::rules {
void PrimitiveConcept::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    assert(target_complexity == 1);
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& predicate : factory.get_vocabulary_info()->get_predicates()) {
        if (predicate.get_arity() == 1) {
            auto element = factory.make_primitive_concept(predicate, 0);
            auto denotations = element.get_element()->evaluate(states, caches);
            if (data.m_concept_hash_table.insert(denotations).second) {
                data.m_reprs.push_back(element.compute_repr());
                data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
    }
}

std::string PrimitiveConcept::get_name() const {
    return core::element::PrimitiveConcept::get_name();
}

}
