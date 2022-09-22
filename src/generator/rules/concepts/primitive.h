#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PRIMITIVE_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PRIMITIVE_H_

#include "../rule.h"
#include "../../../core/elements/concepts/primitive.h"


namespace dlplan::generator::rules {

class PrimitiveConcept : public Rule {
public:
    PrimitiveConcept() : Rule() { }

    void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::element::DenotationsCaches& caches) override {
        assert(target_complexity == 1);
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& predicate : factory.get_vocabulary_info_ref().get_predicates_ref()) {
            if (predicate.get_arity() == 1) {
                auto element = factory.make_primitive_concept(predicate, 0);
                auto denotations = element.get_element_ref().evaluate(states, caches);
                if (data.m_concept_hash_table.insert(denotations).second) {
                    data.m_reprs.push_back(element.compute_repr());
                    data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
                    increment_generated();
                }
            }
        }
    }

    std::string get_name() const override {
        return core::element::PrimitiveConcept::get_name();
    }
};

}

#endif
