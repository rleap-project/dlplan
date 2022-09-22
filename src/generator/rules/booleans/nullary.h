#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_NULLARY_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_NULLARY_H_

#include "../rule.h"
#include "../../../core/elements/booleans/nullary.h"


namespace dlplan::generator::rules {

class NullaryBoolean : public Rule {
public:
    NullaryBoolean() : Rule() { }

    void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::element::DenotationsCaches& caches) override {
        assert(target_complexity == 1);
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& predicate : factory.get_vocabulary_info_ref().get_predicates_ref()) {
            if (predicate.get_arity() == 0) {
                auto element = factory.make_nullary_boolean(predicate);
                auto denotations = element.get_element_ref().evaluate(states, caches);
                if (data.m_boolean_hash_table.insert(denotations).second) {
                    data.m_reprs.push_back(element.compute_repr());
                    data.m_booleans_by_iteration[target_complexity].push_back(std::move(element));
                    increment_generated();
                }
            }
        }
    }

    std::string get_name() const override {
        return core::element::NullaryBoolean::get_name();
    }
};

}

#endif
