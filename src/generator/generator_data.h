#ifndef DLPLAN_SRC_GENERATOR_GENERATOR_DATA_H_
#define DLPLAN_SRC_GENERATOR_GENERATOR_DATA_H_

#include <iostream>
#include <numeric>
#include <vector>

#include "hash_table.h"
#include "iteration_data.h"
#include "result_data.h"

namespace dlplan::generator {

struct GeneratorData {
    std::shared_ptr<core::SyntacticElementFactory> m_factory;
    HashTable m_boolean_and_numerical_hash_table;
    HashTable m_concept_hash_table;
    HashTable m_role_hash_table;
    std::vector<IterationData<core::Boolean>> m_boolean_iteration_data;
    std::vector<IterationData<core::Numerical>> m_numerical_iteration_data;
    std::vector<IterationData<core::Concept>> m_concept_iteration_data;
    std::vector<IterationData<core::Role>> m_role_iteration_data;
    ResultData m_result_data;

    GeneratorData(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity)
      : m_factory(factory),
        m_boolean_iteration_data(complexity + 1),
        m_numerical_iteration_data(complexity + 1),
        m_concept_iteration_data(complexity + 1),
        m_role_iteration_data(complexity + 1) { }

    void print_statistics() const {
        std::cout << "Total generated features: " << m_boolean_and_numerical_hash_table.get_cache_hits() + m_boolean_and_numerical_hash_table.get_cache_misses() + m_concept_hash_table.get_cache_hits() + m_concept_hash_table.get_cache_misses() + m_role_hash_table.get_cache_hits() + m_role_hash_table.get_cache_misses() << std::endl
              << "Total novel features: " << m_boolean_and_numerical_hash_table.get_cache_misses() + m_concept_hash_table.get_cache_misses() + m_role_hash_table.get_cache_misses() << std::endl
              << "Total concept elements: " << std::accumulate(m_concept_iteration_data.begin(), m_concept_iteration_data.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl
              << "Total role elements: " << std::accumulate(m_role_iteration_data.begin(), m_role_iteration_data.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl
              << "Total numerical elements: " << std::accumulate(m_numerical_iteration_data.begin(), m_numerical_iteration_data.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl
              << "Total boolean elements: " << std::accumulate(m_boolean_iteration_data.begin(), m_boolean_iteration_data.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl;
    }
};

}

#endif
