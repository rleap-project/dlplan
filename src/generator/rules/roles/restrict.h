#include "../rule.h"


namespace dlplan::generator::rules {

class RestrictRole : public Rule {
protected:
    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& role : data.get_role_elements_by_complexity()[i]) {
                for (const auto& concept : data.get_concept_elements_by_complexity()[j]) {
                    if (data.reached_limit()) return;
                    else if (data.add_role(states, data.get_factory().make_restrict_role(role, concept))) {
                        m_count_instantiations += 1;
                    }
                }
            }
        }
    }

public:
    RestrictRole(bool enabled) : Rule("r_restrict", enabled) { }
};

}
