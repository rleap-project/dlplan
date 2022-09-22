#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_IDENTITY_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_IDENTITY_H_

#include "../rule.h"
#include "../../../core/elements/roles/identity.h"


namespace dlplan::generator::rules {

class IdentityRole : public Rule {
public:
    IdentityRole() : Rule() { }

    void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::element::DenotationsCaches& caches) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& c : data.m_concepts_by_iteration[target_complexity-1]) {
            auto element = factory.make_identity_role(c);
            auto denotations = element.get_element_ref().evaluate(states, caches);
            if (data.m_role_hash_table.insert(denotations).second) {
                data.m_reprs.push_back(element.compute_repr());
                data.m_roles_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
    }

    std::string get_name() const override {
        return core::element::IdentityRole::get_name();
    }
};

}

#endif
