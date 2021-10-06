#include "../rule.h"


namespace dlplan::generator::rules {

class OneOfConcept : public Rule {
public:
    OneOfConcept() : Rule("c_one_of") { }

    virtual void generate(const States& states, int, FeatureGeneratorData& data) override {
        for (const auto& constant : data.get_factory().get_vocabulary_info()->get_constants()) {
            if (data.add_concept(states, data.get_factory().make_one_of_concept(constant))) {
                m_count_instantiations += 1;
            }
        }
    }
};

}
