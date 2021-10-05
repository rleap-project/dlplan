#include "../rule.h"


namespace dlplan::generator::rules {

class PrimitiveRole : public Rule {
protected:
    virtual void generate_impl(const States& states, int, FeatureGeneratorData& data) override {
        const std::vector<core::Predicate>& predicates = data.get_factory().get_vocabulary_info()->get_predicates();
        for (const auto& predicate : predicates) {
            for (int pos1 = 0; pos1 < predicate.get_arity(); ++pos1) {
                for (int pos2 = pos1 + 1; pos2 < predicate.get_arity(); ++pos2) {
                    if (data.add_role(states, data.get_factory().make_primitive_role(predicate, pos1, pos2))) {
                        m_count_instantiations += 1;
                    }
                }
            }
        }
    }

public:
    PrimitiveRole(bool enabled) : Rule("r_primitive", enabled) { }
};

}
