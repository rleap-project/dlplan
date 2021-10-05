#include "../rule.h"


namespace dlplan::generator::rules {

class ProjectionConcept : public Rule {
protected:
    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (const auto& role : data.get_role_elements_by_complexity()[iteration]) {
            for (int pos = 0; pos < 2; ++pos) {
                if (data.reached_limit()) return;
                else {
                    if (data.add_concept(states, data.get_factory().make_projection_concept(role, pos))) {
                        m_count_instantiations += 1;
                    }
                }
            }
        }
    }

public:
    ProjectionConcept(bool enabled) : Rule("c_projection", enabled) { }
};

}
