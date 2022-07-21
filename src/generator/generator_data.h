#ifndef DLPLAN_SRC_GENERATOR_GENERATOR_DATA_H_
#define DLPLAN_SRC_GENERATOR_GENERATOR_DATA_H_

#include <iostream>
#include <numeric>
#include <vector>

#include "../utils/countdown_timer.h"

#include "hash_utils.h"
#include "../utils/hash_utils.h"

#include "../../include/dlplan/core.h"


namespace dlplan::generator {

struct GeneratorData {
    std::shared_ptr<core::SyntacticElementFactory> m_factory;
    std::unordered_set<std::array<uint32_t, 4>> m_boolean_and_numerical_hash_table;
    std::unordered_set<std::array<uint32_t, 4>> m_concept_hash_table;
    std::unordered_set<std::array<uint32_t, 4>> m_role_hash_table;
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

    // statistics
    int m_num_generated_features;
    int m_num_novel_features;

    GeneratorData(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit, int feature_limit)
      : m_factory(factory),
        m_booleans_by_iteration(std::vector<std::vector<core::Boolean>>(complexity + 1)),
        m_numericals_by_iteration(std::vector<std::vector<core::Numerical>>(complexity + 1)),
        m_concepts_by_iteration(std::vector<std::vector<core::Concept>>(complexity + 1)),
        m_roles_by_iteration(std::vector<std::vector<core::Role>>(complexity + 1)),
        m_complexity(complexity),
        m_time_limit(time_limit),
        m_feature_limit(feature_limit),
        m_timer(time_limit),
        m_num_generated_features(0),
        m_num_novel_features(0) { }

    void print_statistics() const {
        std::cout << "Total generated features: " << m_num_generated_features << std::endl
              << "Total novel features: " << m_num_novel_features << std::endl
              << "Total concept elements: " << std::accumulate(m_concepts_by_iteration.begin(), m_concepts_by_iteration.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl
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
