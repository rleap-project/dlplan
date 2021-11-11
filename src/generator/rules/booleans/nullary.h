#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_NULLARY_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_NULLARY_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class NullaryBoolean : public Rule {
public:
    NullaryBoolean() : Rule("b_nullary") { }

    virtual void generate(const States& states, int, FeatureGeneratorData& data) override {
        const std::vector<core::Predicate>& predicates = data.get_factory().get_vocabulary_info()->get_predicates();
        for (const auto& predicate : predicates) {
            if (predicate.get_arity() == 0) {
                if (data.add_boolean(states, data.get_factory().make_nullary_boolean(predicate))) {
                    m_count_instantiations += 1;
                }
            }
        }
    }
};

}

#endif
