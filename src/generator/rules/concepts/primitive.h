#include "../rule.h"


namespace dlplan::generator::rules {

class PrimitiveConcept : public Rule {
protected:
    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data) override {
        const std::vector<core::Predicate>& predicates = data.get_factory().get_vocabulary_info()->get_predicates();
        for (const auto& predicate : predicates) {
            for (int pos = 0; pos < predicate.get_arity(); ++pos) {
                if (data.add_concept(states, data.get_factory().make_primitive_concept(predicate, pos))) {
                    m_count_instantiations += 1;
                }
            }
        }
    }

public:
    PrimitiveConcept(bool enabled) : Rule("c_primitive", enabled) { }
};

}