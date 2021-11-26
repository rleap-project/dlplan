#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_RESTRICT_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_RESTRICT_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class RestrictRole : public Rule {
public:
    RestrictRole() : Rule("r_restrict") { }

    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data) override {
        if (iteration == 2) {
            for (const auto& role : data.get_role_elements_by_complexity()[1]) {
                for (const auto& concept : data.get_concept_elements_by_complexity()[1]) {
                    if (data.reached_limit()) return;
                    else if (data.add_role(states, data.get_factory().make_restrict_role(role, concept))) {
                        m_count_instantiations += 1;
                    }
                }
            }
        }
    }
};

}

#endif
