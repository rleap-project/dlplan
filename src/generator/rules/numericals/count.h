#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_COUNT_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_COUNT_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class CountNumerical : public Rule {
public:
    CountNumerical() : Rule("n_count") { }

    virtual void generate(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (const auto& concept : data.get_concept_elements_by_complexity()[iteration]) {
            if (data.reached_limit()) return;
            else if (data.add_numerical(states, data.get_factory().make_count(concept))) {
                m_count_instantiations += 1;
            }
        }
        for (const auto& role : data.get_role_elements_by_complexity()[iteration]) {
            if (data.reached_limit()) return;
            else if (data.add_numerical(states, data.get_factory().make_count(role))) {
                m_count_instantiations += 1;
            }
        }
    }
};

}

#endif
