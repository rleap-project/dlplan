#include "../../include/dlplan/generator.h"

#include "feature_generator.h"


namespace dlplan::generator {

FeatureGenerator::FeatureGenerator() : m_pImpl(FeatureGeneratorImpl()) { }

FeatureGenerator::FeatureGenerator(const FeatureGenerator& other)
    : m_pImpl(*other.m_pImpl) { }

FeatureGenerator& FeatureGenerator::operator=(const FeatureGenerator& other) {
    if (this != &other) {
        *m_pImpl = *other.m_pImpl;
    }
    return *this;
}
FeatureGenerator::FeatureGenerator(FeatureGenerator&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

FeatureGenerator& FeatureGenerator::operator=(FeatureGenerator&& other) {
    if (this != &other) {
        std::swap(*m_pImpl, *other.m_pImpl);
    }
    return *this;
}

FeatureGenerator::~FeatureGenerator() = default;

FeatureRepresentations FeatureGenerator::generate(
    core::SyntacticElementFactory& factory,
    int concept_complexity_limit,
    int role_complexity_limit,
    int boolean_complexity_limit,
    int count_numerical_complexity_limit,
    int distance_numerical_complexity_limit,
    int time_limit,
    int feature_limit,
    int num_threads,
    const core::States& states) {
    return m_pImpl->generate(factory, concept_complexity_limit, role_complexity_limit, boolean_complexity_limit, count_numerical_complexity_limit, distance_numerical_complexity_limit, time_limit, feature_limit, num_threads, states);
}

void FeatureGenerator::set_generate_empty_boolean(bool enable) {
    m_pImpl->set_generate_empty_boolean(enable);
}

void FeatureGenerator::set_generate_inclusion_boolean(bool enable) {
    m_pImpl->set_generate_inclusion_boolean(enable);
}

void FeatureGenerator::set_generate_nullary_boolean(bool enable) {
    m_pImpl->set_generate_nullary_boolean(enable);
}

void FeatureGenerator::set_generate_all_concept(bool enable) {
    m_pImpl->set_generate_all_concept(enable);
}

void FeatureGenerator::set_generate_and_concept(bool enable) {
    m_pImpl->set_generate_and_concept(enable);
}

void FeatureGenerator::set_generate_bot_concept(bool enable) {
    m_pImpl->set_generate_bot_concept(enable);
}

void FeatureGenerator::set_generate_diff_concept(bool enable) {
    m_pImpl->set_generate_diff_concept(enable);
}

void FeatureGenerator::set_generate_equal_concept(bool enable) {
    m_pImpl->set_generate_equal_concept(enable);
}

void FeatureGenerator::set_generate_not_concept(bool enable) {
    m_pImpl->set_generate_not_concept(enable);
}

void FeatureGenerator::set_generate_one_of_concept(bool enable) {
    m_pImpl->set_generate_one_of_concept(enable);
}

void FeatureGenerator::set_generate_or_concept(bool enable) {
    m_pImpl->set_generate_or_concept(enable);
}

void FeatureGenerator::set_generate_primitive_concept(bool enable) {
    m_pImpl->set_generate_primitive_concept(enable);
}

void FeatureGenerator::set_generate_projection_concept(bool enable) {
    m_pImpl->set_generate_projection_concept(enable);
}

void FeatureGenerator::set_generate_some_concept(bool enable) {
    m_pImpl->set_generate_some_concept(enable);
}

void FeatureGenerator::set_generate_subset_concept(bool enable) {
    m_pImpl->set_generate_subset_concept(enable);
}

void FeatureGenerator::set_generate_top_concept(bool enable) {
    m_pImpl->set_generate_top_concept(enable);
}

void FeatureGenerator::set_generate_concept_distance_numerical(bool enable) {
    m_pImpl->set_generate_concept_distance_numerical(enable);
}

void FeatureGenerator::set_generate_count_numerical(bool enable) {
    m_pImpl->set_generate_count_numerical(enable);
}

void FeatureGenerator::set_generate_and_role(bool enable) {
    m_pImpl->set_generate_and_role(enable);
}

void FeatureGenerator::set_generate_compose_role(bool enable) {
    m_pImpl->set_generate_compose_role(enable);
}

void FeatureGenerator::set_generate_diff_role(bool enable) {
    m_pImpl->set_generate_diff_role(enable);
}

void FeatureGenerator::set_generate_identity_role(bool enable) {
    m_pImpl->set_generate_identity_role(enable);
}

void FeatureGenerator::set_generate_inverse_role(bool enable) {
    m_pImpl->set_generate_inverse_role(enable);
}

void FeatureGenerator::set_generate_not_role(bool enable) {
    m_pImpl->set_generate_not_role(enable);
}

void FeatureGenerator::set_generate_or_role(bool enable) {
    m_pImpl->set_generate_or_role(enable);
}

void FeatureGenerator::set_generate_primitive_role(bool enable) {
    m_pImpl->set_generate_primitive_role(enable);
}

void FeatureGenerator::set_generate_restrict_role(bool enable) {
    m_pImpl->set_generate_restrict_role(enable);
}

void FeatureGenerator::set_generate_top_role(bool enable) {
    m_pImpl->set_generate_top_role(enable);
}

void FeatureGenerator::set_generate_transitive_closure_role(bool enable) {
    m_pImpl->set_generate_transitive_closure_role(enable);
}

void FeatureGenerator::set_generate_transitive_reflexive_closure_role(bool enable) {
    m_pImpl->set_generate_transitive_reflexive_closure_role(enable);
}


}
