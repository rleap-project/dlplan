#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_INCLUSION_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_INCLUSION_H_

#include "../rule.h"
#include "../../../core/elements/booleans/inclusion.h"

namespace dlplan::generator::rules {

class InclusionBoolean : public Rule {
public:
    InclusionBoolean() : Rule() { }

    void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (int i = 1; i < target_complexity - 1; ++i) {
            int j = target_complexity - i - 1;
            for (const auto& c1 : data.m_concepts_by_iteration[i]) {
                for (const auto& c2 : data.m_concepts_by_iteration[j]) {
                    auto element = factory.make_inclusion_boolean(c1, c2);
                    auto denotations = element.get_element_ref().evaluate(states, caches);
                    if (data.m_boolean_hash_table.insert(denotations).second) {
                        data.m_reprs.push_back(element.compute_repr());
                        data.m_booleans_by_iteration[target_complexity].push_back(std::move(element));
                        increment_generated();
                    }
                }
            }
        }
        for (int i = 1; i < target_complexity - 1; ++i) {
            int j = target_complexity - i - 1;
            for (const auto& r1 : data.m_roles_by_iteration[i]) {
                for (const auto& r2 : data.m_roles_by_iteration[j]) {
                    auto element = factory.make_inclusion_boolean(r1, r2);
                    auto denotations = element.get_element_ref().evaluate(states, caches);
                    if (data.m_boolean_hash_table.insert(denotations).second) {
                        data.m_reprs.push_back(element.compute_repr());
                        data.m_booleans_by_iteration[target_complexity].push_back(std::move(element));
                        increment_generated();
                    }
                }
            }
        }
    }

    std::string get_name() const override {
        return core::element::InclusionBoolean<int>::get_name();
    }
};

}

#endif
