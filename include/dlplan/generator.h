#ifndef DLPLAN_INCLUDE_DLPLAN_GENERATOR_H_
#define DLPLAN_INCLUDE_DLPLAN_GENERATOR_H_

#include "core.h"


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
    utils::pimpl<FeatureGeneratorImpl> m_pImpl;

public:
    FeatureGenerator();
    ~FeatureGenerator();

    /**
     * Exhaustively generates features with pairwise disjoint feature evaluations on the states.
     */
    FeatureRepresentations generate(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit, int feature_limit, int num_threads, const States& states);

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
    void set_generate_role_distance_numerical(bool enable);
    void set_generate_sum_concept_distance_numerical(bool enable);
    void set_generate_sum_role_distance_numerical(bool enable);
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


}

#endif
