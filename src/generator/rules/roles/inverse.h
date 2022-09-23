#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_INVERSE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_INVERSE_H_

#include "../rule.h"
#include "../../../core/elements/roles/inverse.h"


namespace dlplan::generator::rules {

class InverseRole : public Rule {
public:
    InverseRole() : Rule() { }

    void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& r : data.m_roles_by_iteration[target_complexity-1]) {
            auto element = factory.make_inverse_role(r);
            auto denotations = element.get_element_ref().evaluate(states, caches);
            if (data.m_role_hash_table.insert(denotations).second) {
                data.m_reprs.push_back(element.compute_repr());
                data.m_roles_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
    }

    std::string get_name() const override {
        return core::element::InverseRole::get_name();
    }
};

}

#endif
