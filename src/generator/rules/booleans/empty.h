#include "../rule.h"


namespace dlplan::generator::rules {

class EmptyBoolean : public Rule {
protected:
    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (const auto& concept : data.get_concept_elements_by_complexity()[iteration]) {
            if (data.reached_limit()) return;
            else if (data.add_boolean(states, data.get_factory().make_empty_boolean(concept))) {
                m_count_instantiations += 1;
            }
        }
        for (const auto& role : data.get_role_elements_by_complexity()[iteration]) {
            if (data.reached_limit()) return;
            else if (data.add_boolean(states, data.get_factory().make_empty_boolean(role))) {
                m_count_instantiations += 1;
            }
        }
    }

public:
    EmptyBoolean(bool enabled) : Rule("b_empty", enabled) { }
};

}