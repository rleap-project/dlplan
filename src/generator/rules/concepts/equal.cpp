#include "equal.h"

#include "../../generator_data.h"

#include "../../../../include/dlplan/core/elements/roles/primitive.h"


namespace dlplan::generator::rules {
void EqualConcept::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    if (target_complexity == 3)
    {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (int i = 1; i < target_complexity - 1; ++i)
        {
            int j = target_complexity - i - 1;
            for (const auto& r1 : data.m_roles_by_iteration[i])
            {
                auto r1_primitive_role = std::dynamic_pointer_cast<const core::PrimitiveRole>(r1);
                if (r1_primitive_role)
                {
                    std::string r1_predicate_name = r1_primitive_role->get_predicate().get_name();
                    for (const auto& r2 : data.m_roles_by_iteration[j])
                    {
                        auto r2_primitive_role = std::dynamic_pointer_cast<const core::PrimitiveRole>(r2);
                        if (r2_primitive_role)
                        {
                            std::string r2_predicate_name = r2_primitive_role->get_predicate().get_name();
                            if ((r1_predicate_name) == r2_predicate_name + "_g") {
                                auto element = factory.make_equal_concept(r2, r1);
                                auto denotations = element->evaluate(states, caches);
                                if (data.m_concept_hash_table.insert(denotations).second)
                                {
                                    std::get<2>(data.m_generated_features).push_back(element);
                                    data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
                                    increment_generated();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

std::string EqualConcept::get_name() const {
    return "c_equal";
}

}
