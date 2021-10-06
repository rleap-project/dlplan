#include "../rule.h"


namespace dlplan::generator::rules {

class BotConcept : public Rule {
public:
    BotConcept() : Rule("c_bot") { }

    virtual void generate(const States& states, int, FeatureGeneratorData& data) override {
        if (data.add_concept(states, data.get_factory().make_bot_concept())) {
            m_count_instantiations += 1;
        }
    }
};

}
