#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_COUNT_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_COUNT_H_

#include "../rule.h"
#include "../../../core/elements/numericals/count.h"


namespace dlplan::generator::rules {

class CountNumerical : public Rule {
public:
    CountNumerical() : Rule() { }

    void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& concept : data.m_concepts_by_iteration[target_complexity-1]) {
            auto element = factory.make_count_numerical(concept);
            auto denotations = element.get_element_ref().evaluate(states, caches);
            if (data.m_numerical_hash_table.insert(denotations).second) {
                data.m_reprs.push_back(element.compute_repr());
                data.m_numericals_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
        for (const auto& role : data.m_roles_by_iteration[target_complexity-1]) {
            auto element = factory.make_count_numerical(role);
            auto denotations = element.get_element_ref().evaluate(states, caches);
            if (data.m_numerical_hash_table.insert(denotations).second) {
                data.m_reprs.push_back(element.compute_repr());
                data.m_numericals_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
    }

    std::string get_name() const override {
        return core::element::CountNumerical<int>::get_name();
    }
};

}

#endif
