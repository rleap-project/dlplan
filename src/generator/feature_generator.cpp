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

#include "../../include/dlplan/generator.h"
#include "../utils/logging.h"


namespace dlplan::generator {


FeatureGeneratorImpl::FeatureGeneratorImpl(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit, int feature_limit,
    bool generate_empty_boolean,
    bool generate_all_concept,
    bool generate_and_concept,
    bool generate_bot_concept,
    bool generate_diff_concept,
    bool generate_equal_concept,
    bool generate_not_concept,
    bool generate_one_of_concept,
    bool generate_or_concept,
    bool generate_primitive_concept,
    bool generate_projection_concept,
    bool generate_some_concept,
    bool generate_subset_concept,
    bool generate_top_concept,
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
    bool generate_primitive_role,
    bool generate_restrict_role,
    bool generate_top_role,
    bool generate_transitive_closure_role,
    bool generate_transitive_reflexive_closure_role)
    : m_factory(factory), m_complexity(complexity), m_time_limit(time_limit), m_feature_limit(feature_limit) { 
        if (generate_one_of_concept) m_primitive_rules.emplace_back(std::make_unique<rules::OneOfConcept>());  // one of is the simplest primitive to interpret.
        if (generate_primitive_concept) m_primitive_rules.emplace_back(std::make_unique<rules::PrimitiveConcept>());
        if (generate_primitive_role) m_primitive_rules.emplace_back(std::make_unique<rules::PrimitiveRole>());
        if (generate_bot_concept) m_primitive_rules.emplace_back(std::make_unique<rules::BotConcept>());
        if (generate_top_concept) m_primitive_rules.emplace_back(std::make_unique<rules::TopConcept>());
        if (generate_top_role) m_primitive_rules.emplace_back(std::make_unique<rules::TopRole>());

        if (generate_count_numerical) m_inductive_rules.emplace_back(std::make_unique<rules::CountNumerical>());
        if (generate_concept_distance_numerical) m_inductive_rules.emplace_back(std::make_unique<rules::ConceptDistanceNumerical>());
        if (generate_role_distance_numerical) m_inductive_rules.emplace_back(std::make_unique<rules::RoleDistanceNumerical>());
        if (generate_sum_concept_distance_numerical) m_inductive_rules.emplace_back(std::make_unique<rules::SumConceptDistanceNumerical>());
        if (generate_sum_role_distance_numerical) m_inductive_rules.emplace_back(std::make_unique<rules::SumRoleDistanceNumerical>());

        if (generate_empty_boolean) m_inductive_rules.emplace_back(std::make_unique<rules::EmptyBoolean>());

        if (generate_projection_concept) m_inductive_rules.emplace_back(std::make_unique<rules::ProjectionConcept>());
        if (generate_equal_concept) m_inductive_rules.emplace_back(std::make_unique<rules::EqualConcept>());
        if (generate_and_concept) m_inductive_rules.emplace_back(std::make_unique<rules::AndConcept>());
        if (generate_diff_concept) m_inductive_rules.emplace_back(std::make_unique<rules::DiffConcept>());
        if (generate_or_concept) m_inductive_rules.emplace_back(std::make_unique<rules::OrConcept>());
        if (generate_not_concept) m_inductive_rules.emplace_back(std::make_unique<rules::NotConcept>());
        if (generate_subset_concept) m_inductive_rules.emplace_back(std::make_unique<rules::SubsetConcept>());
        if (generate_some_concept) m_inductive_rules.emplace_back(std::make_unique<rules::SomeConcept>());
        if (generate_all_concept) m_inductive_rules.emplace_back(std::make_unique<rules::AllConcept>());

        if (generate_identity_role) m_inductive_rules.emplace_back(std::make_unique<rules::IdentityRole>());
        if (generate_inverse_role) m_inductive_rules.emplace_back(std::make_unique<rules::InverseRole>());
        if (generate_and_role) m_inductive_rules.emplace_back(std::make_unique<rules::AndRole>());
        if (generate_diff_role) m_inductive_rules.emplace_back(std::make_unique<rules::DiffRole>());
        if (generate_restrict_role) m_inductive_rules.emplace_back(std::make_unique<rules::RestrictRole>());
        if (generate_compose_role) m_inductive_rules.emplace_back(std::make_unique<rules::ComposeRole>());
        if (generate_or_role) m_inductive_rules.emplace_back(std::make_unique<rules::OrRole>());
        if (generate_not_role) m_inductive_rules.emplace_back(std::make_unique<rules::NotRole>());
        if (generate_transitive_closure_role) m_inductive_rules.emplace_back(std::make_unique<rules::TransitiveClosureRole>());
        if (generate_transitive_reflexive_closure_role) m_inductive_rules.emplace_back(std::make_unique<rules::TransitiveReflexiveClosureRole>());
    }


FeatureRepresentations FeatureGeneratorImpl::generate(const States& states) {
    // Resets the counters
    for (const auto& rule : m_primitive_rules) {
        rule->initialize();
    }
    for (const auto& rule : m_inductive_rules) {
        rule->initialize();
    }
    FeatureGeneratorData data(m_factory, m_complexity, m_time_limit, m_feature_limit);
    generate_base(states, data);
    generate_inductively(states, data);
    // utils::g_log << "Overall results: " << std::endl;
    // print_overall_statistics();
    return data.get_feature_reprs();
}

void FeatureGeneratorImpl::generate_base(const States& states, FeatureGeneratorData& data) {
    utils::g_log << "Started generating base features of complexity 1." << std::endl;
    for (const auto& rule : m_primitive_rules) {
        rule->generate(states, 0, data);
    }
    utils::g_log << "Complexity " << 1 << ":" << std::endl;
    data.print_statistics();
    print_brief_statistics();
    utils::g_log << "Finished generating base features." << std::endl;
}

void FeatureGeneratorImpl::generate_inductively(const States& states, FeatureGeneratorData& data) {
    utils::g_log << "Started generating composite features." << std::endl;
    for (int iteration = 1; iteration < m_complexity; ++iteration) {  // every composition adds at least one complexity
        if (data.reached_limit()) break;        
        for (const auto& rule : m_inductive_rules) {
            rule->generate(states, iteration, data);
        }
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

}
