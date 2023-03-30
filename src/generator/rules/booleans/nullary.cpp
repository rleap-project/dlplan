#include "nullary.h"

#include "../../generator_data.h"

#include "../../../core/elements/booleans/nullary.h"


namespace dlplan::generator::rules {
void NullaryBoolean::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    assert(target_complexity == 1);
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& predicate : factory.get_vocabulary_info()->get_predicates()) {
        if (predicate.get_arity() == 0) {
            auto element = factory.make_nullary_boolean(predicate);
            auto denotations = element.get_element()->evaluate(states, caches);
            if (data.m_boolean_and_numerical_hash_table.insert(denotations).second) {
                data.m_reprs.push_back(element.compute_repr());
                data.m_booleans_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
    }
}

std::string NullaryBoolean::get_name() const {
    return core::element::NullaryBoolean::get_name();
}

}
