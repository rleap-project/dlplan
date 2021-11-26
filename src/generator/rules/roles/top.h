#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_TOP_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_TOP_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class TopRole : public Rule {
public:
    TopRole() : Rule("r_top") { }

    virtual void generate_impl(const States& states, int, FeatureGeneratorData& data) override {
        if (data.add_role(states, data.get_factory().make_top_role())) {
            m_count_instantiations += 1;
        }
    }
};

}

#endif
