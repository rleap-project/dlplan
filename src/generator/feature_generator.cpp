#include "feature_generator.h"

#include "generator_data.h"

#include "../../include/dlplan/core.h"
#include "../utils/logging.h"

#include <iostream>


namespace dlplan::generator {


FeatureGeneratorImpl::FeatureGeneratorImpl()
    : c_one_of(std::make_shared<rules::OneOfConcept>()),
      c_top(std::make_shared<rules::TopConcept>()),
      c_bot(std::make_shared<rules::BotConcept>()),
      c_primitive(std::make_shared<rules::PrimitiveConcept>()),
      r_top(std::make_shared<rules::TopRole>()),
      r_primitive(std::make_shared<rules::PrimitiveRole>()),
      b_nullary(std::make_shared<rules::NullaryBoolean>()),
      b_empty(std::make_shared<rules::EmptyBoolean>()),
      n_count(std::make_shared<rules::CountNumerical>()),
      b_inclusion(std::make_shared<rules::InclusionBoolean>()),
      n_concept_distance(std::make_shared<rules::ConceptDistanceNumerical>()),
      c_and(std::make_shared<rules::AndConcept>()),
      c_or(std::make_shared<rules::OrConcept>()),
      c_not(std::make_shared<rules::NotConcept>()),
      c_diff(std::make_shared<rules::DiffConcept>()),
      c_projection(std::make_shared<rules::ProjectionConcept>()),
      c_equal(std::make_shared<rules::EqualConcept>()),
      c_subset(std::make_shared<rules::SubsetConcept>()),
      c_some(std::make_shared<rules::SomeConcept>()),
      c_all(std::make_shared<rules::AllConcept>()),
      r_and(std::make_shared<rules::AndRole>()),
      r_or(std::make_shared<rules::OrRole>()),
      r_not(std::make_shared<rules::NotRole>()),
      r_diff(std::make_shared<rules::DiffRole>()),
      r_identity(std::make_shared<rules::IdentityRole>()),
      r_inverse(std::make_shared<rules::InverseRole>()),
      r_restrict(std::make_shared<rules::RestrictRole>()),
      r_compose(std::make_shared<rules::ComposeRole>()),
      r_transitive_closure(std::make_shared<rules::TransitiveClosureRole>()),
      r_transitive_reflexive_closure(std::make_shared<rules::TransitiveReflexiveClosureRole>()) {
    m_primitive_rules.emplace_back(b_nullary);
    m_primitive_rules.emplace_back(c_one_of);
    m_primitive_rules.emplace_back(c_top);
    m_primitive_rules.emplace_back(c_bot);
    m_primitive_rules.emplace_back(c_primitive);
    m_primitive_rules.emplace_back(r_top);
    m_primitive_rules.emplace_back(r_primitive);

    m_concept_inductive_rules.emplace_back(c_and);
    m_concept_inductive_rules.emplace_back(c_or);
    m_concept_inductive_rules.emplace_back(c_not);
    m_concept_inductive_rules.emplace_back(c_diff);
    m_concept_inductive_rules.emplace_back(c_projection);
    m_concept_inductive_rules.emplace_back(c_equal);
    m_concept_inductive_rules.emplace_back(c_subset);
    m_concept_inductive_rules.emplace_back(c_some);
    m_concept_inductive_rules.emplace_back(c_all);

    m_role_inductive_rules.emplace_back(r_restrict);
    m_role_inductive_rules.emplace_back(r_and);
    m_role_inductive_rules.emplace_back(r_or);
    m_role_inductive_rules.emplace_back(r_not);
    m_role_inductive_rules.emplace_back(r_diff);
    m_role_inductive_rules.emplace_back(r_identity);
    m_role_inductive_rules.emplace_back(r_inverse);
    m_role_inductive_rules.emplace_back(r_compose);
    m_role_inductive_rules.emplace_back(r_transitive_closure);
    m_role_inductive_rules.emplace_back(r_transitive_reflexive_closure);

    m_boolean_inductive_rules.emplace_back(b_empty);
    m_boolean_inductive_rules.emplace_back(b_inclusion);

    m_numerical_inductive_rules.emplace_back(n_count);
    m_numerical_inductive_rules.emplace_back(n_concept_distance);
}

FeatureGeneratorImpl::FeatureGeneratorImpl(const FeatureGeneratorImpl& other) = default;

FeatureGeneratorImpl& FeatureGeneratorImpl::operator=(const FeatureGeneratorImpl& other) = default;

FeatureGeneratorImpl::FeatureGeneratorImpl(FeatureGeneratorImpl&& other) = default;

FeatureGeneratorImpl& FeatureGeneratorImpl::operator=(FeatureGeneratorImpl&& other) = default;

FeatureGeneratorImpl::~FeatureGeneratorImpl() = default;

FeatureRepresentations FeatureGeneratorImpl::generate(
    core::SyntacticElementFactory& factory,
    const core::States& states,
    int concept_complexity_limit,
    int role_complexity_limit,
    int boolean_complexity_limit,
    int count_numerical_complexity_limit,
    int distance_numerical_complexity_limit,
    int time_limit,
    int feature_limit) {
    // Initialize statistics in each rule.
    for (auto& r : m_primitive_rules) r->initialize();
    for (auto& r : m_concept_inductive_rules) r->initialize();
    for (auto& r : m_role_inductive_rules) r->initialize();
    for (auto& r : m_boolean_inductive_rules) r->initialize();
    for (auto& r : m_numerical_inductive_rules) r->initialize();
    // Initialize memory to store intermediate results.
    GeneratorData data(factory, std::max({concept_complexity_limit, role_complexity_limit, boolean_complexity_limit, count_numerical_complexity_limit, distance_numerical_complexity_limit}), time_limit, feature_limit);
    // Initialize cache.
    core::DenotationsCaches caches;
    generate_base(states, data, caches);
    generate_inductively(states, concept_complexity_limit, role_complexity_limit, boolean_complexity_limit, count_numerical_complexity_limit, distance_numerical_complexity_limit, data, caches);
    return data.m_reprs;
}

void FeatureGeneratorImpl::generate_base(
    const core::States& states,
    GeneratorData& data,
    core::DenotationsCaches& caches) {
    utils::g_log << "Started generating base features of complexity 1." << std::endl;
    for (const auto& rule : m_primitive_rules) {
        if (data.reached_resource_limit()) break;
        rule->generate(states, 1, data, caches);
    }
    utils::g_log << "Complexity " << 1 << ":" << std::endl;
    print_statistics();
    utils::g_log << "Finished generating base features." << std::endl;
}

void FeatureGeneratorImpl::generate_inductively(
    const core::States& states,
    int concept_complexity_limit,
    int role_complexity_limit,
    int boolean_complexity_limit,
    int count_numerical_complexity_limit,
    int distance_numerical_complexity_limit,
    GeneratorData& data,
    core::DenotationsCaches& caches) {
    utils::g_log << "Started generating composite features. " << std::endl;
    int max_complexity = std::max({concept_complexity_limit, role_complexity_limit, boolean_complexity_limit, count_numerical_complexity_limit, distance_numerical_complexity_limit});
    for (int target_complexity = 2; target_complexity <= max_complexity; ++target_complexity) {  // every composition adds at least one complexity
        if (target_complexity <= concept_complexity_limit) {
            if (data.reached_resource_limit()) break;
            for (const auto& rule : m_concept_inductive_rules) {
                if (data.reached_resource_limit()) break;
                rule->generate(states, target_complexity, data, caches);
            }
        }
        if (target_complexity <= role_complexity_limit) {
            if (data.reached_resource_limit()) break;
            for (const auto& rule : m_role_inductive_rules) {
                if (data.reached_resource_limit()) break;
                rule->generate(states, target_complexity, data, caches);
            }
        }
        if (target_complexity <= boolean_complexity_limit) {
            if (data.reached_resource_limit()) break;
            for (const auto& rule : m_boolean_inductive_rules) {
                if (data.reached_resource_limit()) break;
                rule->generate(states, target_complexity, data, caches);
            }
        }
        if (target_complexity <= count_numerical_complexity_limit) {
            if (data.reached_resource_limit()) break;
            n_count->generate(states, target_complexity, data, caches);
        }
        if (target_complexity <= distance_numerical_complexity_limit) {
            if (data.reached_resource_limit()) break;
            n_concept_distance->generate(states, target_complexity, data, caches);
        }
        utils::g_log << "Complexity " << target_complexity << ":" << std::endl;
        data.print_statistics();
        print_statistics();
    }
    utils::g_log << "Finished generating composite features." << std::endl;
}

void FeatureGeneratorImpl::print_statistics() const {
    for (auto& r : m_primitive_rules) r->print_statistics();
    for (auto& r : m_concept_inductive_rules) r->print_statistics();
    for (auto& r : m_role_inductive_rules) r->print_statistics();
    for (auto& r : m_boolean_inductive_rules) r->print_statistics();
    for (auto& r : m_numerical_inductive_rules) r->print_statistics();
}

void FeatureGeneratorImpl::set_generate_empty_boolean(bool enable) {
    b_empty->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_inclusion_boolean(bool enable) {
    b_inclusion->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_nullary_boolean(bool enable) {
    b_nullary->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_all_concept(bool enable) {
    c_all->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_and_concept(bool enable) {
    c_and->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_bot_concept(bool enable) {
    c_bot->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_diff_concept(bool enable) {
    c_diff->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_equal_concept(bool enable) {
    c_equal->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_not_concept(bool enable) {
    c_not->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_one_of_concept(bool enable) {
    c_one_of->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_or_concept(bool enable) {
    c_or->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_primitive_concept(bool enable) {
    c_primitive->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_projection_concept(bool enable) {
    c_projection->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_some_concept(bool enable) {
    c_some->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_subset_concept(bool enable) {
    c_subset->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_top_concept(bool enable) {
    c_top->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_concept_distance_numerical(bool enable) {
    n_concept_distance->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_count_numerical(bool enable) {
    n_count->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_and_role(bool enable) {
    r_and->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_compose_role(bool enable) {
    r_compose->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_diff_role(bool enable) {
    r_diff->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_identity_role(bool enable) {
    r_identity->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_inverse_role(bool enable) {
    r_inverse->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_not_role(bool enable) {
    r_not->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_or_role(bool enable) {
    r_or->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_primitive_role(bool enable) {
    r_primitive->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_restrict_role(bool enable) {
    r_restrict->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_top_role(bool enable) {
    r_top->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_transitive_closure_role(bool enable) {
    r_transitive_closure->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_transitive_reflexive_closure_role(bool enable) {
    r_transitive_reflexive_closure->set_enabled(enable);
}


}
