#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_OR_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_OR_H_

#include "../rule.h"
#include "../../../core/elements/concepts/or.h"


namespace dlplan::generator::rules {

class OrConcept : public Rule {
public:
    OrConcept() : Rule() { }

    void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::element::DenotationsCaches& caches) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (int i = 1; i < target_complexity - 1; ++i) {
            int j = target_complexity - i - 1;
            for (const auto& c1 : data.m_concepts_by_iteration[i]) {
                for (const auto& c2 : data.m_concepts_by_iteration[j]) {
                    auto element = factory.make_or_concept(c1, c2);
                    auto denotations = element.get_element_ref().evaluate(states, caches);
                    if (data.m_concept_hash_table.insert(denotations).second) {
                        data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
                        increment_generated();
                    }
                }
            }
        }
    }

    std::string get_name() const override {
        return core::element::OrConcept::get_name();
    }
};

}

#endif
