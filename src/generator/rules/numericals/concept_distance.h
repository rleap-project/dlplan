#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_CONCEPT_DISTANCE_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_CONCEPT_DISTANCE_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class ConceptDistanceNumerical : public Rule {
public:
    ConceptDistanceNumerical() : Rule("n_concept_distance") { }

    virtual void generate(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (int i = 1; i < iteration; ++i) {
            for (int j = 1; j < iteration - i; ++j) {
                int k = iteration - i - j;
                for (const auto& concept_left : data.get_concept_elements_by_complexity()[i]) {
                    for (const auto& role : data.get_role_elements_by_complexity()[j]) {
                        for (const auto& concept_right : data.get_concept_elements_by_complexity()[k]) {
                            if (data.reached_limit()) return;
                            else if (data.add_numerical(states, data.get_factory().make_concept_distance(concept_left, role, concept_right))) {
                                m_count_instantiations += 1;
                            }
                        }
                    }
                }
            }
        }
    }
};

}

#endif
