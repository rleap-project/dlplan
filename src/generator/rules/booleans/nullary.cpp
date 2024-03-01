#include "nullary.h"

#include "../../generator_data.h"


namespace dlplan::generator::rules {
void NullaryBoolean::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    assert(target_complexity == 1);
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& predicate : factory.get_vocabulary_info()->get_predicates()) {
        if (predicate.get_arity() == 0) {
            auto element = factory.make_nullary_boolean(predicate);
            auto denotations = element->evaluate(states, caches);
            if (data.m_boolean_hash_table.insert(denotations).second) {
                std::get<0>(data.m_generated_features).push_back(element);
                data.m_booleans_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
    }
}

std::string NullaryBoolean::get_name() const {
    return "b_nullary";
}

}
