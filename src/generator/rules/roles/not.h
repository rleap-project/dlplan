#include "../rule.h"


namespace dlplan::generator::rules {

class NotRole : public Rule {
protected:
    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (const auto& role : data.get_role_elements_by_complexity()[iteration]) {
            if (data.reached_limit()) return;
            else if (data.add_role(states, data.get_factory().make_not_role(role))) {
                m_count_instantiations += 1;
            }
        }
    }

public:
    NotRole(bool enabled) : Rule("r_not", enabled) { }
};

}
