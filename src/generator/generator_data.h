#ifndef DLPLAN_SRC_GENERATOR_GENERATOR_DATA_H_
#define DLPLAN_SRC_GENERATOR_GENERATOR_DATA_H_

#include "../utils/countdown_timer.h"
#include "../../include/dlplan/core.h"
#include "../../include/dlplan/generator.h"

#include <iostream>
#include <numeric>
#include <vector>


namespace dlplan::generator {

struct GeneratorData {
    core::SyntacticElementFactory& m_factory;
    std::unordered_set<std::shared_ptr<const core::BooleanDenotations>> m_boolean_hash_table;
    std::unordered_set<std::shared_ptr<const core::NumericalDenotations>> m_numerical_hash_table;
    std::unordered_set<std::shared_ptr<const core::ConceptDenotations>> m_concept_hash_table;
    std::unordered_set<std::shared_ptr<const core::RoleDenotations>> m_role_hash_table;
    std::vector<std::vector<std::shared_ptr<const core::Boolean>>> m_booleans_by_iteration;
    std::vector<std::vector<std::shared_ptr<const core::Numerical>>> m_numericals_by_iteration;
    std::vector<std::vector<std::shared_ptr<const core::Concept>>> m_concepts_by_iteration;
    std::vector<std::vector<std::shared_ptr<const core::Role>>> m_roles_by_iteration;
    GeneratedFeatures m_generated_features;

    // resource constraints
    int m_complexity;
    int m_time_limit;
    int m_feature_limit;
    utils::CountdownTimer m_timer;

    GeneratorData(
      core::SyntacticElementFactory& factory,
      int complexity,
      int time_limit,
      int feature_limit)
      : m_factory(factory),
        m_booleans_by_iteration(std::vector<std::vector<std::shared_ptr<const core::Boolean>>>(complexity + 1)),
        m_numericals_by_iteration(std::vector<std::vector<std::shared_ptr<const core::Numerical>>>(complexity + 1)),
        m_concepts_by_iteration(std::vector<std::vector<std::shared_ptr<const core::Concept>>>(complexity + 1)),
        m_roles_by_iteration(std::vector<std::vector<std::shared_ptr<const core::Role>>>(complexity + 1)),
        m_complexity(complexity),
        m_time_limit(time_limit),
        m_feature_limit(feature_limit),
        m_timer(time_limit) { }

    int get_num_features() {
      return std::get<0>(m_generated_features).size() + std::get<1>(m_generated_features).size() + std::get<2>(m_generated_features).size() + std::get<3>(m_generated_features).size();
    }

    void print_statistics() const {
      utils::g_log << "Total concept elements: " << std::accumulate(m_concepts_by_iteration.begin(), m_concepts_by_iteration.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl
                   << "Total role elements: " << std::accumulate(m_roles_by_iteration.begin(), m_roles_by_iteration.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl
                   << "Total numerical elements: " << std::accumulate(m_numericals_by_iteration.begin(), m_numericals_by_iteration.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl
                   << "Total boolean elements: " << std::accumulate(m_booleans_by_iteration.begin(), m_booleans_by_iteration.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl;
    }

    bool reached_resource_limit() {
      return (get_num_features() >= m_feature_limit || m_timer.is_expired());
    }
};

}

#endif
