#include "../rule.h"


namespace dlplan::generator::rules {

class TopRole : public Rule {
protected:
    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data) override {
        if (data.add_role(states, data.get_factory().make_top_role())) {
            m_count_instantiations += 1;
        }
    }

public:
    TopRole(bool enabled) : Rule("r_top", enabled) { }
};

}
