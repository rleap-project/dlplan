#include "feature_generator.h"

#include <numeric>
#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>


#include "rules/concepts/all.h"
#include "rules/concepts/and.h"
#include "rules/concepts/bot.h"
#include "rules/concepts/diff.h"
#include "rules/concepts/equal.h"
#include "rules/concepts/not.h"
#include "rules/concepts/one_of.h"
#include "rules/concepts/or.h"
#include "rules/concepts/primitive.h"
#include "rules/concepts/projection.h"
#include "rules/concepts/some.h"
#include "rules/concepts/subset.h"
#include "rules/concepts/top.h"

#include "rules/roles/and.h"
#include "rules/roles/compose.h"
#include "rules/roles/diff.h"
#include "rules/roles/identity.h"
#include "rules/roles/inverse.h"
#include "rules/roles/not.h"
#include "rules/roles/or.h"
#include "rules/roles/primitive.h"
#include "rules/roles/restrict.h"
#include "rules/roles/top.h"
#include "rules/roles/transitive_closure.h"
#include "rules/roles/transitive_reflexive_closure.h"

#include "rules/numericals/concept_distance.h"
#include "rules/numericals/count.h"
#include "rules/numericals/role_distance.h"
#include "rules/numericals/sum_concept_distance.h"
#include "rules/numericals/sum_role_distance.h"

#include "rules/booleans/empty.h"
#include "rules/booleans/nullary.h"

#include "hash_table.h"
#include "generator_data.h"

#include "../../include/dlplan/generator.h"
#include "../utils/logging.h"
#include "../utils/threadpool.h"


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
      n_role_distance(std::make_shared<rules::RoleDistanceNumerical>()),
      n_sum_concept_distance(std::make_shared<rules::SumConceptDistanceNumerical>()),
      n_sum_role_distance(std::make_shared<rules::SumRoleDistanceNumerical>()),
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
    m_primitive_rules.emplace_back(c_one_of);
    m_primitive_rules.emplace_back(c_top);
    m_primitive_rules.emplace_back(c_bot);
    m_primitive_rules.emplace_back(c_primitive);
    m_primitive_rules.emplace_back(r_top);
    m_primitive_rules.emplace_back(r_primitive);

    m_inductive_rules.emplace_back(b_nullary);
    m_inductive_rules.emplace_back(b_empty);
    m_inductive_rules.emplace_back(n_count);
    m_inductive_rules.emplace_back(b_inclusion);
    m_inductive_rules.emplace_back(n_concept_distance);
    m_inductive_rules.emplace_back(n_role_distance);
    m_inductive_rules.emplace_back(n_sum_concept_distance);
    m_inductive_rules.emplace_back(n_sum_role_distance);

    m_inductive_rules.emplace_back(c_and);
    m_inductive_rules.emplace_back(c_or);
    m_inductive_rules.emplace_back(c_not);
    m_inductive_rules.emplace_back(c_diff);
    m_inductive_rules.emplace_back(c_projection);
    m_inductive_rules.emplace_back(c_equal);
    m_inductive_rules.emplace_back(c_subset);
    m_inductive_rules.emplace_back(c_some);
    m_inductive_rules.emplace_back(c_all);

    m_inductive_rules.emplace_back(r_and);
    m_inductive_rules.emplace_back(r_or);
    m_inductive_rules.emplace_back(r_not);
    m_inductive_rules.emplace_back(r_diff);
    m_inductive_rules.emplace_back(r_identity);
    m_inductive_rules.emplace_back(r_inverse);
    m_inductive_rules.emplace_back(r_restrict);
    m_inductive_rules.emplace_back(r_compose);
    m_inductive_rules.emplace_back(r_transitive_closure);
    m_inductive_rules.emplace_back(r_transitive_reflexive_closure);
}

FeatureRepresentations FeatureGeneratorImpl::generate(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit, int feature_limit, const States& states) {
    for (auto& r : m_primitive_rules) r->get_stats().initialize();
    for (auto& r : m_inductive_rules) r->get_stats().initialize();
    GeneratorData data(factory, complexity);
    // Initialize default threadpool
    utils::threadpool::ThreadPool th;
    generate_base(states, data, th);
    generate_inductively(complexity, states, data, th);
    // utils::g_log << "Overall results: " << std::endl;
    // print_overall_statistics();
    return data.m_reprs;
}

void FeatureGeneratorImpl::generate_base(const States& states, GeneratorData& data, utils::threadpool::ThreadPool& th) {
    utils::g_log << "Started generating base features of complexity 1." << std::endl;
    std::vector<utils::threadpool::ThreadPool::TaskFuture<void>> tasks;
    for (const auto& rule : m_primitive_rules) {
        rule->submit_tasks(states, 0, data, th);
    }
    for (const auto& rule : m_primitive_rules) {
        rule->parse_results_of_tasks(0, data);
    }
    utils::g_log << "Complexity " << 1 << ":" << std::endl;
    print_brief_statistics();
    utils::g_log << "Finished generating base features." << std::endl;
}

void FeatureGeneratorImpl::generate_inductively(int complexity, const States& states, GeneratorData& data, utils::threadpool::ThreadPool& th) {
    utils::g_log << "Started generating composite features." << std::endl;
    for (int iteration = 1; iteration < complexity; ++iteration) {  // every composition adds at least one complexity
        for (const auto& rule : m_inductive_rules) {
            rule->submit_tasks(states, iteration, data, th);
        }
        for (const auto& rule : m_inductive_rules) {
            rule->parse_results_of_tasks(iteration, data);
        }
        utils::g_log << "Complexity " << iteration+1 << ":" << std::endl;
        data.print_statistics();
        print_brief_statistics();
    }
    utils::g_log << "Finished generating composite features." << std::endl;
}

void FeatureGeneratorImpl::print_brief_statistics() const {
    for (auto& r : m_primitive_rules) r->print_statistics();
    for (auto& r : m_inductive_rules) r->print_statistics();
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

void FeatureGeneratorImpl::set_generate_role_distance_numerical(bool enable) {
    n_role_distance->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_sum_concept_distance_numerical(bool enable) {
    n_sum_concept_distance->set_enabled(enable);
}

void FeatureGeneratorImpl::set_generate_sum_role_distance_numerical(bool enable) {
    n_sum_role_distance->set_enabled(enable);
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
