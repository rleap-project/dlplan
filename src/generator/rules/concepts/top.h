#include "../rule.h"


namespace dlplan::generator::rules {

class TopConcept : public Rule {
protected:
    virtual void generate_impl(const States& states, int, FeatureGeneratorData& data) override {
        if (data.add_concept(states, data.get_factory().make_top_concept())) {
            m_count_instantiations += 1;
        }
    }

public:
    TopConcept(bool enabled) : Rule("c_top", enabled) { }
};

}
