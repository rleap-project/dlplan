#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_OR_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_OR_H_

#include "../rule.h"
#include "../../../core/elements/roles/or.h"


namespace dlplan::generator::rules {

class OrRole : public Rule {
public:
    OrRole() : Rule() { }

    void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (int i = 1; i < target_complexity - 1; ++i) {
            int j = target_complexity - i - 1;
            for (const auto& r1 : data.m_roles_by_iteration[i]) {
                for (const auto& r2 : data.m_roles_by_iteration[j]) {
                    auto element = factory.make_or_role(r1, r2);
                    auto denotations = element.get_element_ref().evaluate(states, caches);
                    if (data.m_role_hash_table.insert(denotations).second) {
                        data.m_reprs.push_back(element.compute_repr());
                        data.m_roles_by_iteration[target_complexity].push_back(std::move(element));
                        increment_generated();
                    }
                }
            }
        }
    }

    std::string get_name() const override {
        return core::element::OrRole::get_name();
    }
};

}

#endif
