#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PROJECTION_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PROJECTION_H_

#include "../rule.h"
#include "../../../core/elements/concepts/projection.h"


namespace dlplan::generator::rules {

class ProjectionConcept : public Rule {
public:
    ProjectionConcept() : Rule() { }

    void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& r : data.m_roles_by_iteration[target_complexity-1]) {
            for (int pos = 0; pos < 2; ++pos) {
                auto element = factory.make_projection_concept(r, pos);
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
        return core::element::ProjectionConcept::get_name();
    }
};

}

#endif
