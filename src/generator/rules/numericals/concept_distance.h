#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_CONCEPT_DISTANCE_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_CONCEPT_DISTANCE_H_

#include "../rule.h"


namespace dlplan::generator::rules {
class ConceptDistanceNumerical : public Rule {
public:
    void generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) override;

    std::string get_name() const override;
};

}

#endif
