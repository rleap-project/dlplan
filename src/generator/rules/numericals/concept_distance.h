#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_CONCEPT_DISTANCE_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_CONCEPT_DISTANCE_H_

#include "../rule.h"
#include "../../../core/elements/numericals/concept_distance.h"
#include "../../../core/elements/element.h"


namespace dlplan::generator::rules {



class ConceptDistanceNumerical : public Rule {
public:
    ConceptDistanceNumerical() : Rule() { }

    void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        int j = 3;  // R:C has complexity 3
        for (int i = 1; i < target_complexity - j - 1; ++i) {
            int k = target_complexity - i - j - 1;
            for (const auto& c1 : data.m_concepts_by_iteration[i]) {
                // left role must evaluate to concept denotation that contains exactly one object.
                auto c1_denotations = c1.get_element_ref().evaluate(states, caches);
                bool one = true;
                for (const auto denot_ptr : *c1_denotations) {
                    if (denot_ptr->count() != 1) {
                        one = false;
                        break;
                    }
                }
                if (!one) {
                    continue;
                }
                for (const auto& r : data.m_roles_by_iteration[j]) {
                    // middle role must be restriction
                    if (r.compute_repr().substr(0, 10) != "r_restrict") {
                        continue;
                    }
                    for (const auto& c2 : data.m_concepts_by_iteration[k]) {
                        auto element = factory.make_concept_distance_numerical(c1, r, c2);
                        auto denotations = element.get_element_ref().evaluate(states, caches);
                        if (data.m_numerical_hash_table.insert(denotations).second) {
                            data.m_reprs.push_back(element.compute_repr());
                            data.m_numericals_by_iteration[target_complexity].push_back(std::move(element));
                            increment_generated();
                        }
                    }
                }
            }
        }
    }


    std::string get_name() const override {
        return core::element::ConceptDistanceNumerical::get_name();
    }
};

}

#endif
