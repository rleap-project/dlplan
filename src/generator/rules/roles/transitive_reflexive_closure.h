#include "../rule.h"


namespace dlplan::generator::rules {

class TransitiveReflexiveClosureRole : public Rule {
public:
    TransitiveReflexiveClosureRole() : Rule("r_transitive_reflexive_closure") { }

    virtual void generate(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (const auto& role : data.get_role_elements_by_complexity()[iteration]) {
            if (data.reached_limit()) return;
            else if (data.add_role(states, data.get_factory().make_transitive_reflexive_closure(role))) {
                m_count_instantiations += 1;
            }
        }
    }
};

}
