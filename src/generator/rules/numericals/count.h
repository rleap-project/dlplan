#include "../rule.h"


namespace dlplan::generator::rules {

class CountNumerical : public Rule {
protected:
    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (const auto& concept : data.get_concept_elements_by_complexity()[iteration]) {
            if (data.reached_limit()) return;
            else if (data.add_numerical(states, data.get_factory().make_count(concept))) {
                m_count_instantiations += 1;
            }
        }
        for (const auto& role : data.get_role_elements_by_complexity()[iteration]) {
            if (data.reached_limit()) return;
            else if (data.add_numerical(states, data.get_factory().make_count(role))) {
                m_count_instantiations += 1;
            }
        }
    }

public:
    CountNumerical(bool enabled) : Rule("c_count", enabled) { }
};

}
