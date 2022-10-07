#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_DIFF_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_DIFF_H_

#include "../rule.h"
#include "../../../core/elements/roles/diff.h"

namespace dlplan::generator::rules {

class DiffRole : public Rule {
public:
    DiffRole() : Rule() { }

    void generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) override;

    std::string get_name() const override;
};

}

#endif
