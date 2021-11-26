#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_BOT_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_BOT_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class BotConcept : public Rule {
public:
    BotConcept() : Rule("c_bot") { }

    virtual void generate_impl(const States& states, int, FeatureGeneratorData& data) override {
        if (data.add_concept(states, data.get_factory().make_bot_concept())) {
            m_count_instantiations += 1;
        }
    }
};

}

#endif
