#include "../rule.h"


namespace dlplan::generator::rules {

class NotConcept : public Rule {
protected:
    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (const auto& concept : data.get_concept_elements_by_complexity()[iteration]) {
            if (data.reached_limit()) return;
            else if (data.add_concept(states, data.get_factory().make_not_concept(concept))) {
                m_count_instantiations += 1;
            }
        }
    }

public:
    NotConcept(bool enabled) : Rule("c_not", enabled) { }
};

}
