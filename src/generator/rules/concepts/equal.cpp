#include "equal.h"

#include "../../generator_data.h"

#include "../../../core/elements/concepts/equal.h"
#include "../../../core/elements/roles/primitive.h"


namespace dlplan::generator::rules {
void EqualConcept::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    if (target_complexity == 3) {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (int i = 1; i < target_complexity - 1; ++i) {
            int j = target_complexity - i - 1;
            for (const auto& r1 : data.m_roles_by_iteration[i]) {
                auto primitive_role = std::dynamic_pointer_cast<const core::element::PrimitiveRole>(r1.get_element());
                if (primitive_role) {
                    std::string predicate_name = primitive_role->get_predicate_ref().get_name_ref();
                    if (predicate_name.substr(predicate_name.size() - 2, 2) != "_g") {
                        continue;
                    }
                    for (const auto& r2 : data.m_roles_by_iteration[j]) {
                        auto element = factory.make_equal_concept(r2, r1);
                        auto denotations = element.get_element_ref().evaluate(states, caches);
                        if (data.m_concept_hash_table.insert(denotations).second) {
                            data.m_reprs.push_back(element.compute_repr());
                            data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
                            increment_generated();
                        }
                    }
                }
            }
        }
    }
}

std::string EqualConcept::get_name() const {
    return core::element::EqualConcept::get_name();
}

}
