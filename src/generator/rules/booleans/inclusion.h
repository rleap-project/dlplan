#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_INCLUSION_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_INCLUSION_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class InclusionBoolean : public Rule {
public:
    InclusionBoolean() : Rule("b_inclusion") { }

    virtual void generate_impl(const States& states, int, FeatureGeneratorData& data) override {
    }
};

}

#endif
