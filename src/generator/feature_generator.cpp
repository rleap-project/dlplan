#include "feature_generator.h"

#include <numeric>
#include <iostream>
#include <cassert>

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

#include "../../include/dlplan/generator.h"
#include "../utils/logging.h"
#include "../utils/threadpool.h"

namespace dlplan::generator {


FeatureGeneratorImpl::FeatureGeneratorImpl()
    : c_one_of(std::make_shared<rules::OneOfConcept>(rules::OneOfConcept())),
      c_top(std::make_shared<rules::TopConcept>(rules::TopConcept())),
      c_bot(std::make_shared<rules::BotConcept>(rules::BotConcept())),
      c_primitive(std::make_shared<rules::PrimitiveConcept>(rules::PrimitiveConcept())),
      r_top(std::make_shared<rules::TopRole>(rules::TopRole())),
      r_primitive(std::make_shared<rules::PrimitiveRole>(rules::PrimitiveRole())),
      b_nullary(std::make_shared<rules::NullaryBoolean>(rules::NullaryBoolean())),
      b_empty(std::make_shared<rules::EmptyBoolean>(rules::EmptyBoolean())),
      n_count(std::make_shared<rules::CountNumerical>(rules::CountNumerical())),
      b_inclusion(std::make_shared<rules::InclusionBoolean>(rules::InclusionBoolean())),
      n_concept_distance(std::make_shared<rules::ConceptDistanceNumerical>(rules::ConceptDistanceNumerical())),
      n_role_distance(std::make_shared<rules::RoleDistanceNumerical>(rules::RoleDistanceNumerical())),
      n_sum_concept_distance(std::make_shared<rules::SumConceptDistanceNumerical>(rules::SumConceptDistanceNumerical())),
      n_sum_role_distance(std::make_shared<rules::SumRoleDistanceNumerical>(rules::SumRoleDistanceNumerical())),
      c_and(std::make_shared<rules::AndConcept>(rules::AndConcept())),
      c_or(std::make_shared<rules::OrConcept>(rules::OrConcept())),
      c_not(std::make_shared<rules::NotConcept>(rules::NotConcept())),
      c_diff(std::make_shared<rules::DiffConcept>(rules::DiffConcept())),
      c_projection(std::make_shared<rules::ProjectionConcept>(rules::ProjectionConcept())),
      c_equal(std::make_shared<rules::EqualConcept>(rules::EqualConcept())),
      c_subset(std::make_shared<rules::SubsetConcept>(rules::SubsetConcept())),
      c_some(std::make_shared<rules::SomeConcept>(rules::SomeConcept())),
      c_all(std::make_shared<rules::AllConcept>(rules::AllConcept())),
      r_and(std::make_shared<rules::AndRole>(rules::AndRole())),
      r_or(std::make_shared<rules::OrRole>(rules::OrRole())),
      r_not(std::make_shared<rules::NotRole>(rules::NotRole())),
      r_diff(std::make_shared<rules::DiffRole>(rules::DiffRole())),
      r_identity(std::make_shared<rules::IdentityRole>(rules::IdentityRole())),
      r_inverse(std::make_shared<rules::InverseRole>(rules::InverseRole())),
      r_restrict(std::make_shared<rules::RestrictRole>(rules::RestrictRole())),
      r_compose(std::make_shared<rules::ComposeRole>(rules::ComposeRole())),
      r_transitive_closure(std::make_shared<rules::TransitiveClosureRole>(rules::TransitiveClosureRole())),
      r_transitive_reflexive_closure(std::make_shared<rules::TransitiveReflexiveClosureRole>(rules::TransitiveReflexiveClosureRole())) {
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
    // Resets the counters
    for (const auto& rule : m_primitive_rules) {
        rule->initialize();
    }
    for (const auto& rule : m_inductive_rules) {
        rule->initialize();
    }
    FeatureGeneratorData data(factory, complexity, time_limit, feature_limit);
    utils::threadpool::ThreadPool th;
    generate_base(states, data, th);
    generate_inductively(complexity, states, data, th);
    // utils::g_log << "Overall results: " << std::endl;
    // print_overall_statistics();
    return data.get_feature_reprs();
}

void FeatureGeneratorImpl::generate_base(const States& states, FeatureGeneratorData& data, utils::threadpool::ThreadPool& th) {
    utils::g_log << "Started generating base features of complexity 1." << std::endl;
    for (const auto& rule : m_primitive_rules) {
        rule->generate(states, 0, data, th);
    }
    utils::g_log << "Complexity " << 1 << ":" << std::endl;
    data.print_statistics();
    print_brief_statistics();
    utils::g_log << "Finished generating base features." << std::endl;
}

void FeatureGeneratorImpl::generate_inductively(int complexity, const States& states, FeatureGeneratorData& data, utils::threadpool::ThreadPool& th) {
    utils::g_log << "Started generating composite features." << std::endl;
    // Initialize default threadpool
    utils::threadpool::ThreadPool th;
    for (int iteration = 1; iteration < complexity; ++iteration) {  // every composition adds at least one complexity
        if (data.reached_limit()) break;
        // TODO(dominik): Add tasks to threadpool queue
        for (const auto& rule : m_inductive_rules) {
            rule->generate(states, iteration, data, th);
        }
        // TODO(dominik): sleep main thread until queue is empty.
        while (!th.get_queue().empty()) { }
        utils::g_log << "Complexity " << iteration+1 << ":" << std::endl;
        data.print_statistics();
        print_brief_statistics();
    }
    utils::g_log << "Finished generating composite features." << std::endl;
}

void FeatureGeneratorImpl::print_brief_statistics() const {
    for (const auto& rule : m_primitive_rules) {
        rule->print_statistics();
    }
    for (const auto& rule : m_inductive_rules) {
        rule->print_statistics();
    }
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
