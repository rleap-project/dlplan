#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ALL_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ALL_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class AllConcept : public Rule {
public:
    AllConcept() : Rule("c_all") { }

    virtual void generate(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& role : data.get_role_elements_by_complexity()[i]) {
                for (const auto& concept : data.get_concept_elements_by_complexity()[j]) {
                    if (data.reached_limit()) return;
                    else if (data.add_concept(states, data.get_factory().make_all_concept(role, concept))) {
                        m_count_instantiations += 1;
                    }
                }
            }
        }
    }
};

}

#endif
