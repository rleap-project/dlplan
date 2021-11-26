#include "feature_generator_data.h"

#include <numeric>

#include "../utils/logging.h"
#include "hash_tables/hash_table_lossless.h"
#include "hash_tables/hash_table_sha-256.h"
#include "hash_tables/hash_table_murmur.h"


namespace dlplan::generator {

template<typename T>
static void print_elements(const std::vector<std::vector<T>>& elements_by_complexity) {
    for (const auto& elements : elements_by_complexity) {
        for (const auto& element : elements) {
            std::cout << element.compute_complexity() << " " << element.compute_repr() << std::endl;
        }
    }
}

/**
 * Evaluates an element on a collection of states.
 */
template<typename D>
std::vector<D> evaluate(core::Element<D>& element, const States& states) {
    std::vector<D> result;
    result.reserve(states.size());
    for (const auto& state : states) {
        result.push_back(element.evaluate(state));
    }
    result.shrink_to_fit();
    return result;
}


/**
 * Flattens a vector of concept denotations
 */
static std::vector<int> flatten_concept_denotation(const std::vector<std::vector<int>>& denotations) {
    std::vector<int> result;
    // the layout
    result.push_back(denotations.size());
    for (const auto& denot : denotations) {
        result.push_back(denot.size());
    }
    // the data
    for (const auto& denot : denotations) {
        result.insert(result.end(), denot.begin(), denot.end());
    }
    result.shrink_to_fit();
    return result;
}


/**
 * Flattens a vector of role denotations
 */
static std::vector<int> flatten_role_denotation(const std::vector<std::vector<std::pair<int, int>>>& denotations) {
    std::vector<int> result;
    // the layout
    result.push_back(denotations.size());
    for (const auto& denot : denotations) {
        result.push_back(denot.size());
    }
    // the data
    for (const auto& denot : denotations) {
        for (const auto& p : denot) {
            result.push_back(p.first);
            result.push_back(p.second);
        }
    }
    result.shrink_to_fit();
    return result;
}


FeatureGeneratorData::FeatureGeneratorData(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit, int feature_limit) :
      m_factory(factory), m_complexity(complexity), m_time_limit(time_limit), m_feature_limit(feature_limit),
      m_concept_elements_by_complexity(std::vector<std::vector<dlplan::core::Concept>>(complexity+1)),
      m_role_elements_by_complexity(std::vector<std::vector<dlplan::core::Role>>(complexity+1)),
      m_numerical_elements_by_complexity(std::vector<std::vector<dlplan::core::Numerical>>(complexity+1)),
      m_boolean_elements_by_complexity(std::vector<std::vector<dlplan::core::Boolean>>(complexity+1)),
      m_hash_table(std::make_unique<HashTableMurmur>()),
      m_timer(time_limit) { }


bool FeatureGeneratorData::reached_limit() const {
    return (m_timer.is_expired() || (m_count_features >= m_feature_limit));
}

bool FeatureGeneratorData::add_concept(const States& states, core::Concept&& concept) {
    const std::vector<int>& denotation = flatten_concept_denotation(evaluate<core::ConceptDenotation>(concept, states));
    if (m_hash_table->insert_concept(denotation)) {
        m_concept_elements_by_complexity[concept.compute_complexity()].emplace_back(concept);
        m_feature_reprs.push_back(concept.compute_repr());
        ++m_count_features;
        return true;
    }
    return false;
}

bool FeatureGeneratorData::add_role(const States& states, core::Role&& role) {
    const std::vector<int>& denotation = flatten_role_denotation(evaluate<core::RoleDenotation>(role, states));
    if (m_hash_table->insert_role(denotation)) {
        m_role_elements_by_complexity[role.compute_complexity()].emplace_back(role);
        m_feature_reprs.push_back(role.compute_repr());
        ++m_count_features;
        return true;
    }
    return false;
}

bool FeatureGeneratorData::add_numerical(const States& states, core::Numerical&& numerical) {
    const std::vector<int>& denotation = evaluate<int>(numerical, states);
    if (m_hash_table->insert_numerical(denotation)) {
        m_numerical_elements_by_complexity[numerical.compute_complexity()].emplace_back(numerical);
        m_feature_reprs.push_back(numerical.compute_repr());
        ++m_count_features;
        return true;
    }
    return false;
}

bool FeatureGeneratorData::add_boolean(const States& states, core::Boolean&& boolean) {
    const std::vector<bool>& denotation = evaluate<bool>(boolean, states);
    if (m_hash_table->insert_boolean(denotation)) {
        m_boolean_elements_by_complexity[boolean.compute_complexity()].emplace_back(boolean);
        m_feature_reprs.push_back(boolean.compute_repr());
        ++m_count_features;
        return true;
    }
    return false;
}

void FeatureGeneratorData::print_statistics() const {
    std::cout << "Total generated features: " << m_hash_table->get_cache_hits() + m_hash_table->get_cache_misses() << std::endl
              << "Total novel features: " << m_hash_table->get_cache_misses() << std::endl
              << "Total concept elements: " << std::accumulate(m_concept_elements_by_complexity.begin(), m_concept_elements_by_complexity.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl
              << "Total role elements: " << std::accumulate(m_role_elements_by_complexity.begin(), m_role_elements_by_complexity.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl
              << "Total numerical elements: " << std::accumulate(m_numerical_elements_by_complexity.begin(), m_numerical_elements_by_complexity.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl
              << "Total boolean elements: " << std::accumulate(m_boolean_elements_by_complexity.begin(), m_boolean_elements_by_complexity.end(), 0, [&](int current_sum, const auto& e){ return current_sum + e.size(); }) << std::endl;
}

void FeatureGeneratorData::print_overall_statistics() const {
    std::cout << "Generated concepts (complexity representation):" << std::endl;
    print_elements(m_concept_elements_by_complexity);
    std::cout << "Generated roles (complexity representation):" << std::endl;
    print_elements(m_role_elements_by_complexity);
    std::cout << "Generated numericals (complexity representation):" << std::endl;
    print_elements(m_numerical_elements_by_complexity);
    std::cout << "Generated booleans (complexity representation):" << std::endl;
    print_elements(m_boolean_elements_by_complexity);
}


core::SyntacticElementFactory& FeatureGeneratorData::get_factory() const {
    return *m_factory;
}

const std::vector<std::vector<dlplan::core::Concept>>& FeatureGeneratorData::get_concept_elements_by_complexity() const {
    return m_concept_elements_by_complexity;
}

const std::vector<std::vector<dlplan::core::Role>>& FeatureGeneratorData::get_role_elements_by_complexity() const {
    return m_role_elements_by_complexity;
}

const std::vector<std::vector<dlplan::core::Numerical>>& FeatureGeneratorData::get_numerical_elements_by_complexity() const {
    return m_numerical_elements_by_complexity;
}

const std::vector<std::vector<dlplan::core::Boolean>>& FeatureGeneratorData::get_boolean_elements_by_complexity() const {
    return m_boolean_elements_by_complexity;
}

FeatureRepresentations FeatureGeneratorData::get_feature_reprs() const {
    return m_feature_reprs;
}

}
