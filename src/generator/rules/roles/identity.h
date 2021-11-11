#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_IDENTITY_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_IDENTITY_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class IdentityRole : public Rule {
public:
    IdentityRole() : Rule("r_identity") { }

    virtual void generate(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (const auto& concept : data.get_concept_elements_by_complexity()[iteration]) {
            if (data.reached_limit()) return;
            else if (data.add_role(states, data.get_factory().make_identity_role(concept))) {
                m_count_instantiations += 1;
            }
        }
    }
};

}

#endif
