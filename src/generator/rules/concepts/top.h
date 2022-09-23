#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_TOP_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_TOP_H_

#include "../rule.h"
#include "../../../core/elements/concepts/top.h"


namespace dlplan::generator::rules {

class TopConcept : public Rule {
public:
    TopConcept() : Rule() { }

    void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) override {
        assert(target_complexity == 1);
        core::SyntacticElementFactory& factory = data.m_factory;
        auto element = factory.make_top_concept();
        auto& denotations = element.get_element_ref().evaluate(states, caches);
        if (data.m_c_denots_cache.insert(&denotations).second) {
            data.m_reprs.push_back(element.compute_repr());
            data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }

    std::string get_name() const override {
        return core::element::TopConcept::get_name();
    }
};

}

#endif
