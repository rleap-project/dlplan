#ifndef DLPLAN_INCLUDE_DLP_ELEMENT_GENERATOR_H_
#define DLPLAN_INCLUDE_DLP_ELEMENT_GENERATOR_H_

#include "core.h"
#include "pimpl.h"


namespace dlplan::generator {
class NumericalImpl;
class BooleanImpl;
class FeatureCollectionImpl;
class FeatureCollectionWriterImpl;
class FeatureGeneratorImpl;
class Numerical;
class Boolean;

using States = std::vector<core::State>;
using FeatureRepresentations = std::vector<std::string>;

/**
 * FeatureGenerator exhaustively generates features up to the complexity bound or until the time limit was reached.
 */
class FeatureGenerator {
private:
    pimpl<FeatureGeneratorImpl> m_pImpl;

public:
    FeatureGenerator(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit, int feature_limit,
        bool generate_empty_boolean=true,
        bool generate_all_concept=true,
        bool generate_and_concept=true,
        bool generate_diff_concept=true,
        bool generate_not_concept=true,
        bool generate_or_concept=true,
        bool generate_projection_concept=true,
        bool generate_some_concept=true,
        bool generate_subset_concept=true,
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
        bool generate_restrict_role=true,
        bool generate_transitive_closure_role=true,
        bool generate_transitive_reflexive_closure_role=true);
    ~FeatureGenerator();

    /**
     * Exhaustively generates features with pairwise disjoint feature evaluations on the states.
     */
    FeatureRepresentations generate(const States& states);
};


}

#endif
