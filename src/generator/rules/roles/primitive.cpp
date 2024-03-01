#include "primitive.h"

#include "../../generator_data.h"


namespace dlplan::generator::rules {
void PrimitiveRole::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    assert(target_complexity == 1);
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& predicate : factory.get_vocabulary_info()->get_predicates()) {
        if (predicate.get_arity() == 2) {
            auto element = factory.make_primitive_role(predicate, 0, 1);
            auto denotations = element->evaluate(states, caches);
            if (data.m_role_hash_table.insert(denotations).second) {
                std::get<3>(data.m_generated_features).push_back(element);
                data.m_roles_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
    }
}

std::string PrimitiveRole::get_name() const {
    return "r_primitive";
}

}
