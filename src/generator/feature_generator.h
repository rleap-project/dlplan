#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_IMPL_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_IMPL_H_

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <bitset>

#include "../../include/dlplan/core.h"

#include "types.h"
#include "feature_generator_data.h"

#include "rules/rule.h"


namespace dlplan {
namespace core {
    class SyntacticElementFactory;
}
namespace generator {


class FeatureGeneratorImpl {
private:
    const std::shared_ptr<core::SyntacticElementFactory> m_factory;
    const int m_complexity;
    const int m_time_limit;
    const int m_feature_limit;

    std::vector<std::unique_ptr<rules::Rule>> m_primitive_rules;
    std::vector<std::unique_ptr<rules::Rule>> m_inductive_rules;

private:
    /**
     * Generates all Elements with complexity 1.
     */
    void generate_base(const States& states, FeatureGeneratorData& data);

    /**
     * Inductively generate Elements of higher complexity.
     */
    void generate_inductively(const States& states, FeatureGeneratorData& data);

    /**
     * Print some brief overview.
     */
    void print_brief_statistics() const;

public:
    FeatureGeneratorImpl(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit, int feature_limit,
        bool generate_empty_boolean=true,
        bool generate_all_concept=true,
        bool generate_and_concept=true,
        bool generate_bot_concept=true,
        bool generate_diff_concept=true,
        bool generate_equal_concept=true,
        bool generate_not_concept=true,
        bool generate_one_of_concept=true,
        bool generate_or_concept=true,
        bool generate_primitive_concept=true,
        bool generate_projection_concept=true,
        bool generate_some_concept=true,
        bool generate_subset_concept=true,
        bool generate_top_concept=true,
        bool generate_concept_distance_numerical=true,
        bool generate_count_numerical=true,
        bool generate_role_distance_numerical=true,
        bool generate_sum_concept_distance_numerical=true,
        bool generate_sum_role_distance_numerical=true,
        bool generate_and_role=true,
        bool generate_compose_role=true,
        bool generate_diff_role=true,
        bool generate_identity_role=true,
        bool generate_inverse_role=true,
        bool generate_not_role=true,
        bool generate_or_role=true,
        bool generate_primitive_role=true,
        bool generate_restrict_role=true,
        bool generate_top_role=true,
        bool generate_transitive_closure_role=true,
        bool generate_transitive_reflexive_closure_role=true);

    /**
     * Exhaustively generates features with pairwise disjoint feature evaluations on the states.
     */
    FeatureRepresentations generate(const States& states);
};

}
}


#endif
