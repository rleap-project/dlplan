#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_INCLUSION_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_INCLUSION_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class InclusionBoolean : public Rule {
public:
    InclusionBoolean() : Rule("b_inclusion") { }

    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& concept_left : data.get_concept_elements_by_complexity()[i]) {
                for (const auto& concept_right : data.get_concept_elements_by_complexity()[j]) {
                    if (data.reached_limit()) return;
                    else if (data.add_boolean(states, data.get_factory().make_concept_inclusion_boolean(concept_left, concept_right))) {
                        m_count_instantiations += 1;
                    }
                }
            }
        }
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& role_left : data.get_role_elements_by_complexity()[i]) {
                for (const auto& role_right : data.get_role_elements_by_complexity()[j]) {
                    if (data.reached_limit()) return;
                    else if (data.add_boolean(states, data.get_factory().make_role_inclusion_boolean(role_left, role_right))) {
                        m_count_instantiations += 1;
                    }
                }
            }
        }
    }
};

}

#endif
