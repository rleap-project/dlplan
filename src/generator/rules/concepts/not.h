#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_NOT_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_NOT_H_

#include "../rule.h"
#include "../../../core/elements/concepts/not.h"


namespace dlplan::generator::rules {

class NotConcept : public Rule {
public:
    NotConcept() : Rule() { }

    void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& c : data.m_concepts_by_iteration[target_complexity-1]) {
            auto element = factory.make_not_concept(c);
            auto denotations = element.get_element_ref().evaluate(states, caches);
            if (data.m_concept_hash_table.insert(denotations).second) {
                data.m_reprs.push_back(element.compute_repr());
                data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
    }

    std::string get_name() const override {
        return core::element::NotConcept::get_name();
    }
};

}

#endif
