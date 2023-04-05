#ifndef DLPLAN_INCLUDE_DLPLAN_GENERATOR_H_
#define DLPLAN_INCLUDE_DLPLAN_GENERATOR_H_

#include "core.h"
#include "utils/pimpl.h"

#include <string>
#include <vector>


/**
 * Forward declarations and usings
 */
namespace dlplan::generator {
    class FeatureGeneratorImpl;
    using States = std::vector<core::State>;
    using FeatureRepresentations = std::vector<std::string>;
}


namespace dlplan::generator {
/**
 * FeatureGenerator exhaustively generates features up to the complexity bound or until the time limit was reached.
 */
class FeatureGenerator {
private:
    utils::pimpl<FeatureGeneratorImpl> m_pImpl;

public:
    FeatureGenerator();
    FeatureGenerator(const FeatureGenerator& other);
    FeatureGenerator& operator=(const FeatureGenerator& other);
    FeatureGenerator(FeatureGenerator&& other);
    FeatureGenerator& operator=(FeatureGenerator&& other);
    ~FeatureGenerator();

    /**
     * Exhaustively generates features with pairwise disjoint feature evaluations on the states.
     */
    FeatureRepresentations generate(
        core::SyntacticElementFactory& factory,
        const core::States& states,
        int concept_complexity_limit=9,
        int role_complexity_limit=9,
        int boolean_complexity_limit=9,
        int count_numerical_complexity_limit=9,
        int distance_numerical_complexity_limit=9,
        int time_limit=3600,
        int feature_limit=10000);

    void set_generate_empty_boolean(bool enable);
    void set_generate_inclusion_boolean(bool enable);
    void set_generate_nullary_boolean(bool enable);
    void set_generate_all_concept(bool enable);
    void set_generate_and_concept(bool enable);
    void set_generate_bot_concept(bool enable);
    void set_generate_diff_concept(bool enable);
    void set_generate_equal_concept(bool enable);
    void set_generate_not_concept(bool enable);
    void set_generate_one_of_concept(bool enable);
    void set_generate_or_concept(bool enable);
    void set_generate_primitive_concept(bool enable);
    void set_generate_projection_concept(bool enable);
    void set_generate_some_concept(bool enable);
    void set_generate_subset_concept(bool enable);
    void set_generate_top_concept(bool enable);
    void set_generate_concept_distance_numerical(bool enable);
    void set_generate_count_numerical(bool enable);
    void set_generate_and_role(bool enable);
    void set_generate_compose_role(bool enable);
    void set_generate_diff_role(bool enable);
    void set_generate_identity_role(bool enable);
    void set_generate_inverse_role(bool enable);
    void set_generate_not_role(bool enable);
    void set_generate_or_role(bool enable);
    void set_generate_primitive_role(bool enable);
    void set_generate_restrict_role(bool enable);
    void set_generate_top_role(bool enable);
    void set_generate_transitive_closure_role(bool enable);
    void set_generate_transitive_reflexive_closure_role(bool enable);
};


extern FeatureRepresentations generate_features(
    core::SyntacticElementFactory& factory,
    const core::States& states,
    int concept_complexity_limit=9,
    int role_complexity_limit=9,
    int boolean_complexity_limit=9,
    int count_numerical_complexity_limit=9,
    int distance_numerical_complexity_limit=9,
    int time_limit=3600,
    int feature_limit=10000,
    bool generate_empty_boolean=true,
    bool generate_inclusion_boolean=false,
    bool generate_nullary_boolean=true,
    bool generate_all_concept=true,
    bool generate_and_concept=true,
    bool generate_bot_concept=true,
    bool generate_diff_concept=false,
    bool generate_equal_concept=true,
    bool generate_not_concept=true,
    bool generate_one_of_concept=true,
    bool generate_or_concept=false,
    bool generate_primitive_concept=true,
    bool generate_projection_concept=false,
    bool generate_some_concept=true,
    bool generate_subset_concept=false,
    bool generate_top_concept=true,
    bool generate_concept_distance_numerical=true,
    bool generate_count_numerical=true,
    bool generate_and_role=true,
    bool generate_compose_role=false,
    bool generate_diff_role=false,
    bool generate_identity_role=false,
    bool generate_inverse_role=true,
    bool generate_not_role=false,
    bool generate_or_role=false,
    bool generate_primitive_role=true,
    bool generate_restrict_role=true,
    bool generate_top_role=false,
    bool generate_transitive_closure_role=true,
    bool generate_transitive_reflexive_closure_role=false);
}

#endif
