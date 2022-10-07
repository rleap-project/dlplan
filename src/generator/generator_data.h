#ifndef DLPLAN_SRC_GENERATOR_GENERATOR_DATA_H_
#define DLPLAN_SRC_GENERATOR_GENERATOR_DATA_H_

#include "../utils/countdown_timer.h"

#include "../../include/dlplan/core.h"

#include <iostream>
#include <numeric>
#include <vector>


namespace dlplan::generator {

struct GeneratorData {
    core::SyntacticElementFactory& m_factory;
    std::unordered_set<void*> m_boolean_and_numerical_hash_table;
    std::unordered_set<core::ConceptDenotations*> m_concept_hash_table;
    std::unordered_set<core::RoleDenotations*> m_role_hash_table;
    std::vector<std::vector<core::Boolean>> m_booleans_by_iteration;
    std::vector<std::vector<core::Numerical>> m_numericals_by_iteration;
    std::vector<std::vector<core::Concept>> m_concepts_by_iteration;
    std::vector<std::vector<core::Role>> m_roles_by_iteration;
    std::vector<std::string> m_reprs;

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
        m_booleans_by_iteration(std::vector<std::vector<core::Boolean>>(complexity + 1)),
        m_numericals_by_iteration(std::vector<std::vector<core::Numerical>>(complexity + 1)),
        m_concepts_by_iteration(std::vector<std::vector<core::Concept>>(complexity + 1)),
        m_roles_by_iteration(std::vector<std::vector<core::Role>>(complexity + 1)),
        m_complexity(complexity),
        m_time_limit(time_limit),
        m_feature_limit(feature_limit),
        m_timer(time_limit) { }

    void print_statistics() const {
        std::cout << "Total concept elements: " << std::accumulate(m_concepts_by_iteration.begin(), m_concepts_by_iteration.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl
                  << "Total role elements: " << std::accumulate(m_roles_by_iteration.begin(), m_roles_by_iteration.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl
                  << "Total numerical elements: " << std::accumulate(m_numericals_by_iteration.begin(), m_numericals_by_iteration.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl
                  << "Total boolean elements: " << std::accumulate(m_booleans_by_iteration.begin(), m_booleans_by_iteration.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl;
    }

    bool reached_resource_limit() {
      return (static_cast<int>(m_reprs.size()) >= m_feature_limit || m_timer.is_expired());
    }
};

}

#endif
