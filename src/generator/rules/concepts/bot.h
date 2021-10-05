#include "../rule.h"


namespace dlplan::generator::rules {

class BotConcept : public Rule {
protected:
    virtual void generate_impl(const States& states, int, FeatureGeneratorData& data) override {
        if (data.add_concept(states, data.get_factory().make_bot_concept())) {
            m_count_instantiations += 1;
        }
    }

public:
    BotConcept(bool enabled) : Rule("c_bot", enabled) { }
};

}
