#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_

#include "../rule.h"
#include "../../../core/elements/roles/transitive_reflexive_closure.h"


namespace dlplan::generator::rules {

class TransitiveReflexiveClosureRole : public Rule {
public:
    TransitiveReflexiveClosureRole() : Rule() { }

    void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::element::DenotationsCaches& caches) override {
        if (target_complexity == 2) {
            core::SyntacticElementFactory& factory = data.m_factory;
            for (const auto& r : data.m_roles_by_iteration[target_complexity-1]) {
                auto element = factory.make_transitive_reflexive_closure(r);
                auto denotations = element.get_element_ref().evaluate(states, caches);
                if (data.m_role_hash_table.insert(denotations).second) {
                    data.m_roles_by_iteration[target_complexity].push_back(std::move(element));
                    increment_generated();
                }
            }
        }
    }

    std::string get_name() const override {
        return core::element::TransitiveReflexiveClosureRole::get_name();
    }
};

}

#endif
