#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ONE_OF_H_

#include "../rule.h"
#include "../../../core/elements/concepts/one_of.h"


namespace dlplan::generator::rules {

class OneOfConcept : public Rule {
public:
    OneOfConcept() : Rule() { }

    void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        assert(target_complexity == 1);
        for (const auto& constant : factory.get_vocabulary_info_ref().get_constants_ref()) {
            auto element = factory.make_one_of_concept(constant);
            auto denotations = element.get_element_ref().evaluate(states, caches);
            if (data.m_concept_hash_table.insert(denotations).second) {
                data.m_reprs.push_back(element.compute_repr());
                data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
    }

    std::string get_name() const override {
        return core::element::OneOfConcept::get_name();
    }
};

}

#endif
