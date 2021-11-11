#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_EMPTY_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_EMPTY_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class EmptyBoolean : public Rule {
public:
    EmptyBoolean() : Rule("b_empty") { }

    virtual void generate(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (const auto& concept : data.get_concept_elements_by_complexity()[iteration]) {
            if (data.reached_limit()) return;
            else if (data.add_boolean(states, data.get_factory().make_empty_boolean(concept))) {
                m_count_instantiations += 1;
            }
        }
        for (const auto& role : data.get_role_elements_by_complexity()[iteration]) {
            if (data.reached_limit()) return;
            else if (data.add_boolean(states, data.get_factory().make_empty_boolean(role))) {
                m_count_instantiations += 1;
            }
        }
    }
};

}

#endif
