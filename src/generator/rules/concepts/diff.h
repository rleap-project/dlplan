#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_DIFF_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_DIFF_H_

#include "../rule.h"


namespace dlplan::generator::rules {
class DiffConcept : public Rule {
public:
    void generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) override;

    std::string get_name() const override;
};

}

#endif
