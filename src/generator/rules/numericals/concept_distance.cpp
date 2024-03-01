#include "concept_distance.h"

#include "../../generator_data.h"


namespace dlplan::generator::rules {
void ConceptDistanceNumerical::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    int j = 3;  // R:C has complexity 3
    for (int i = 1; i < target_complexity - j - 1; ++i) {
        int k = target_complexity - i - j - 1;
        for (const auto& c1 : data.m_concepts_by_iteration[i]) {
            // left role must evaluate to concept denotation that contains exactly one object.
            auto c1_denotations = c1->evaluate(states, caches);
            bool one = true;
            for (const auto& denot_ptr : *c1_denotations) {
                if (denot_ptr->size() != 1) {
                    one = false;
                    break;
                }
            }
            if (!one) {
                continue;
            }
            for (const auto& r : data.m_roles_by_iteration[j]) {
                // middle role must be restriction
                if (r->str().substr(0, 10) != "r_restrict") {
                    continue;
                }
                for (const auto& c2 : data.m_concepts_by_iteration[k]) {
                    auto element = factory.make_concept_distance_numerical(c1, r, c2);
                    auto denotations = element->evaluate(states, caches);
                    if (data.m_numerical_hash_table.insert(denotations).second) {
                        std::get<1>(data.m_generated_features).push_back(element);
                        data.m_numericals_by_iteration[target_complexity].push_back(std::move(element));
                        increment_generated();
                    }
                }
            }
        }
    }

    j = 1;  // R has complexity 1
    for (int i = 1; i < target_complexity - j - 1; ++i) {
        int k = target_complexity - i - j - 1;
        for (const auto& c1 : data.m_concepts_by_iteration[i]) {
            // left role must evaluate to concept denotation that contains exactly one object.
            auto c1_denotations = c1->evaluate(states, caches);
            bool one = true;
            for (const auto& denot_ptr : *c1_denotations) {
                if (denot_ptr->size() != 1) {
                    one = false;
                    break;
                }
            }
            if (!one) {
                continue;
            }
            for (const auto& r : data.m_roles_by_iteration[j]) {
                for (const auto& c2 : data.m_concepts_by_iteration[k]) {
                    auto element = factory.make_concept_distance_numerical(c1, r, c2);
                    auto denotations = element->evaluate(states, caches);
                    if (data.m_numerical_hash_table.insert(denotations).second) {
                        std::get<1>(data.m_generated_features).push_back(element);
                        data.m_numericals_by_iteration[target_complexity].push_back(std::move(element));
                        increment_generated();
                    }
                }
            }
        }
    }
}


std::string ConceptDistanceNumerical::get_name() const {
    return "n_concept_distance";
}

}
