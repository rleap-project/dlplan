#include "../../include/dlplan/generator.h"

#include "feature_generator.h"


namespace dlplan::generator {

FeatureGenerator::FeatureGenerator(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit, int feature_limit,
    bool generate_empty_boolean,
    bool generate_all_concept,
    bool generate_and_concept,
    bool generate_diff_concept,
    bool generate_not_concept,
    bool generate_or_concept,
    bool generate_projection_concept,
    bool generate_some_concept,
    bool generate_subset_concept,
    bool generate_concept_distance_numerical,
    bool generate_count_numerical,
    bool generate_role_distance_numerical,
    bool generate_sum_concept_distance_numerical,
    bool generate_sum_role_distance_numerical,
    bool generate_and_role,
    bool generate_compose_role,
    bool generate_diff_role,
    bool generate_identity_role,
    bool generate_inverse_role,
    bool generate_not_role,
    bool generate_or_role,
    bool generate_restrict_role,
    bool generate_transitive_closure_role,
    bool generate_transitive_reflexive_closure_role)
    : m_pImpl(FeatureGeneratorImpl(factory, complexity, time_limit, feature_limit, generate_empty_boolean, generate_all_concept, generate_and_concept, generate_diff_concept, generate_not_concept, generate_or_concept, generate_projection_concept, generate_some_concept, generate_subset_concept, generate_concept_distance_numerical, generate_count_numerical, generate_role_distance_numerical, generate_sum_concept_distance_numerical, generate_sum_role_distance_numerical, generate_and_role, generate_compose_role, generate_diff_role, generate_identity_role, generate_inverse_role, generate_not_role, generate_or_role, generate_restrict_role, generate_transitive_closure_role, generate_transitive_reflexive_closure_role)) { }

FeatureGenerator::~FeatureGenerator() = default;

FeatureRepresentations FeatureGenerator::generate(const States& states) {
    return m_pImpl->generate(states);
}


}
