#include "../rule.h"


namespace dlplan::generator::rules {

class OneOfConcept : public Rule {
protected:
    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (const auto& constant : data.get_factory().get_vocabulary_info()->get_constants()) {
            if (data.add_concept(states, data.get_factory().make_one_of_concept(constant))) {
                m_count_instantiations += 1;
            }
        }
    }

public:
    OneOfConcept(bool enabled) : Rule("c_one_of", enabled) { }
};

}
