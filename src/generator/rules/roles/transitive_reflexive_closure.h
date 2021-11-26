#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class TransitiveReflexiveClosureRole : public Rule {
public:
    TransitiveReflexiveClosureRole() : Rule("r_transitive_reflexive_closure") { }

    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data) override {
        if (iteration == 1) {
            for (const auto& role : data.get_role_elements_by_complexity()[1]) {
                if (data.reached_limit()) return;
                else if (data.add_role(states, data.get_factory().make_transitive_reflexive_closure(role))) {
                    m_count_instantiations += 1;
                }
            }
        }
    }
};

}

#endif
