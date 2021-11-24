#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_EQUAL_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_EQUAL_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class EqualConcept : public Rule {
public:
    EqualConcept() : Rule("c_equal") { }

    virtual void generate(const States& states, int iteration, FeatureGeneratorData& data) override {
        if (iteration == 2) {
            for (const auto& role_left : data.get_role_elements_by_complexity()[1]) {
                int pos1 = 12;  // len("r_primitive(") == 12
                std::string repr1 = role_left.compute_repr();
                int pos2 = repr1.find(',');
                for (const auto& role_right : data.get_role_elements_by_complexity()[1]) {                    
                    std::string repr2 = role_right.compute_repr();
                    // TODO: this is extremely ugly. The alternative is to provide getters, downcast to PrimitiveRole and check using get_name method.
                    if (repr1.size() < 12 
                        || repr2.size() < 12
                        || repr1.substr(0,12) != "r_primitive("
                        || repr2.substr(0,12) != "r_primitive(" 
                        || repr1.substr(pos1, pos2-pos1) + "_g" != repr2.substr(pos1, pos2-pos1+2)) {
                            continue;
                    }
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
