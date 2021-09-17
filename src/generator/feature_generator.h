#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_IMPL_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_IMPL_H_

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <bitset>

#include "../../include/dlplan/core.h"
#include "../utils/countdown_timer.h"

#include "types.h"
#include "hash_tables/hash_table.h"


namespace dlplan {
namespace core {
    class Concept;
    class Role;
    class Numerical;
    class Boolean;
    class SyntacticElementFactory;
}
namespace generator {


class FeatureGeneratorImpl {
private:
    const std::shared_ptr<core::SyntacticElementFactory> m_factory;
    const int m_complexity;
    const int m_time_limit;
    const int m_feature_limit;

    const bool m_generate_empty_boolean;
    const bool m_generate_all_concept;
    const bool m_generate_and_concept;
    const bool m_generate_bot_concept;
    const bool m_generate_diff_concept;
    const bool m_generate_equal_concept;
    const bool m_generate_not_concept;
    const bool m_generate_one_of_concept;
    const bool m_generate_or_concept;
    const bool m_generate_primitive_concept;
    const bool m_generate_projection_concept;
    const bool m_generate_some_concept;
    const bool m_generate_subset_concept;
    const bool m_generate_top_concept;
    const bool m_generate_concept_distance_numerical;
    const bool m_generate_count_numerical;
    const bool m_generate_role_distance_numerical;
    const bool m_generate_sum_concept_distance_numerical;
    const bool m_generate_sum_role_distance_numerical;
    const bool m_generate_and_role;
    const bool m_generate_compose_role;
    const bool m_generate_diff_role;
    const bool m_generate_identity_role;
    const bool m_generate_inverse_role;
    const bool m_generate_not_role;
    const bool m_generate_or_role;
    const bool m_generate_primitive_role;
    const bool m_generate_restrict_role;
    const bool m_generate_top_role;
    const bool m_generate_transitive_closure_role;
    const bool m_generate_transitive_reflexive_closure_role;

    /**
     * Generated features sorted by their complexity.
     */
    std::vector<std::vector<dlplan::core::Concept>> m_concept_elements_by_complexity;
    std::vector<std::vector<dlplan::core::Role>> m_role_elements_by_complexity;
    std::vector<std::vector<dlplan::core::Numerical>> m_numerical_elements_by_complexity;
    std::vector<std::vector<dlplan::core::Boolean>> m_boolean_elements_by_complexity;

    /**
     * For uniqueness checking
     */
    std::unique_ptr<HashTable> m_hash_table;

    /**
     * Terminate generation if timer expired.
     */
    utils::CountdownTimer m_timer;

    /**
     * Some statistics.
     */
    int m_count_features;

private:
    /**
     * Returns true iff the limiting constraints are satisfied, i.e.,
     * time limit or feature limit reached
     */
    bool reached_limit() const;

    /**
     * Adds the respective element if it is syntactically and empirically unique.
     * Additionally, adds information related to them in the FeatureCollection.
     * Those features are the final result of the feature generation.
     */
    void add_concept(const States& states, core::Concept&& concept);
    void add_role(const States& states, core::Role&& role);
    void add_numerical(const States& states, core::Numerical&& numerical, FeatureRepresentations& feature_reprs);
    void add_boolean(const States& states, core::Boolean&& boolean, FeatureRepresentations& feature_reprs);

    /**
     * Generates all Elements with complexity 1.
     */
    void generate_base(const States& states);

    void generate_primitive_concept(const States& states);
    void generate_primitive_roles(const States& states);
    void generate_top_role(const States& states);
    void generate_bot_concept(const States& states);
    void generate_top_concept(const States& states);
    void generate_one_of_concept(const States& states);

    /**
     * Inductively generate Elements of higher complexity.
     */
    void generate_inductively(const States& states, FeatureRepresentations& feature_reprs);

    void generate_empty_boolean(const States& states, int iteration, FeatureRepresentations& feature_reprs);
    void generate_all_concept(const States& states, int iteration);
    void generate_and_concept(const States& states, int iteration);
    void generate_diff_concept(const States& states, int iteration);
    void generate_equal_concept(const States& states, int iteration);
    void generate_not_concept(const States& states, int iteration);
    void generate_or_concept(const States& states, int iteration);
    void generate_projection_concept(const States& states, int iteration);
    void generate_some_concept(const States& states, int iteration);
    void generate_subset_concept(const States& states, int iteration);

    void generate_concept_distance_numerical(const States& states, int iteration, FeatureRepresentations& feature_reprs);
    void generate_count_numerical(const States& states, int iteration, FeatureRepresentations& feature_reprs);
    void generate_role_distance_numerical(const States& states, int iteration, FeatureRepresentations& feature_reprs);
    void generate_sum_concept_distance_numerical(const States& states, int iteration, FeatureRepresentations& feature_reprs);
    void generate_sum_role_distance_numerical(const States& states, int iteration, FeatureRepresentations& feature_reprs);

    void generate_and_role(const States& states, int iteration);
    void generate_compose_role(const States& states, int iteration);
    void generate_diff_role(const States& states, int iteration);
    void generate_identity_role(const States& states, int iteration);
    void generate_inverse_role(const States& states, int iteration);
    void generate_not_role(const States& states, int iteration);
    void generate_or_role(const States& states, int iteration);
    void generate_restrict_role(const States& states, int iteration);
    void generate_transitive_closure_role(const States& states, int iteration);
    void generate_transitive_reflexive_closure_role(const States& states, int iteration);

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

    /**
     * Print some brief overview.
     */
    void print_brief_statistics() const;

    /**
     * Print complete statistics.
     */
    void print_overall_statistics() const;
};

}
}


#endif
