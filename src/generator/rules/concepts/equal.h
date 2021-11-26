#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_EQUAL_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_EQUAL_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class EqualConcept : public Rule {
public:
    EqualConcept() : Rule("c_equal") { }

    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data) override {
        if (iteration == 2) {
            for (const auto& role_left : data.get_role_elements_by_complexity()[1]) {
                // Some further restriction used in D2L where righthandside can only be goal version of left handside.
                // However, this the EqualConcept does not contribute much to the blowup so we don't use it.
                for (const auto& role_right : data.get_role_elements_by_complexity()[1]) { 
                    if (data.reached_limit()) return;
                    else if (data.add_concept(states, data.get_factory().make_equal_concept(role_left, role_right))) {
                        m_count_instantiations += 1;
                    }
                }           
            }
        }
    }
};

}

#endif
