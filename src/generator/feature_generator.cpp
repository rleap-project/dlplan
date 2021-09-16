#include "feature_generator.h"

#include <iostream>
#include <cassert>

#include "../../include/dlplan/generator.h"
#include "../utils/logging.h"
#include "hash_tables/hash_table_lossless.h"
#include "hash_tables/hash_table_sha-256.h"
#include "hash_tables/hash_table_murmur.h"


namespace dlplan::generator {

template<typename T>
static int num_elements(const std::vector<std::vector<T>>& elements_by_complexity) {
    int count = 0;
    for (const auto& elements : elements_by_complexity) {
        count += elements.size();
    }
    return count;
}


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



FeatureGeneratorImpl::FeatureGeneratorImpl(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit, int feature_limit,
    bool generate_empty_boolean,
    bool generate_all_concept,
    bool generate_and_concept,
    bool generate_diff_concept,
    bool generate_equal_concept,
    bool generate_not_concept,
    bool generate_or_concept,
    bool generate_projection_concept,
    bool generate_some_concept,
    bool generate_subset_concept,
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
    bool generate_restrict_role,
    bool generate_transitive_closure_role,
    bool generate_transitive_reflexive_closure_role)
    : m_factory(factory), m_complexity(complexity), m_time_limit(time_limit), m_feature_limit(feature_limit),
      m_generate_empty_boolean(generate_empty_boolean),
      m_generate_all_concept(generate_all_concept),
      m_generate_and_concept(generate_and_concept),
      m_generate_diff_concept(generate_diff_concept),
      m_generate_equal_concept(generate_equal_concept),
      m_generate_not_concept(generate_not_concept),
      m_generate_or_concept(generate_or_concept),
      m_generate_projection_concept(generate_projection_concept),
      m_generate_some_concept(generate_some_concept),
      m_generate_subset_concept(generate_subset_concept),
      m_generate_concept_distance_numerical(generate_concept_distance_numerical),
      m_generate_count_numerical(generate_count_numerical),
      m_generate_role_distance_numerical(generate_role_distance_numerical),
      m_generate_sum_concept_distance_numerical(generate_sum_concept_distance_numerical),
      m_generate_sum_role_distance_numerical(generate_sum_role_distance_numerical),
      m_generate_and_role(generate_and_role),
      m_generate_compose_role(generate_compose_role),
      m_generate_diff_role(generate_diff_role),
      m_generate_identity_role(generate_identity_role),
      m_generate_inverse_role(generate_inverse_role),
      m_generate_not_role(generate_not_role),
      m_generate_or_role(generate_or_role),
      m_generate_restrict_role(generate_restrict_role),
      m_generate_transitive_closure_role(generate_transitive_closure_role),
      m_generate_transitive_reflexive_closure_role(generate_transitive_reflexive_closure_role),
      m_concept_elements_by_complexity(complexity+1),
      m_role_elements_by_complexity(complexity+1),
      m_numerical_elements_by_complexity(complexity+1),
      m_boolean_elements_by_complexity(complexity+1),
      m_hash_table(std::make_unique<HashTableMurmur>()),
      m_timer(time_limit),
      m_count_features(0) { }

FeatureRepresentations FeatureGeneratorImpl::generate(const States& states) {
    FeatureRepresentations feature_reprs;
    generate_base(states);
    generate_inductively(states, feature_reprs);
    utils::g_log << "Overall results: " << std::endl;
    // print_overall_statistics();
    return feature_reprs;
}


void FeatureGeneratorImpl::generate_base(const States& states) {
    utils::g_log << "Started generating base features of complexity 1." << std::endl;
    generate_primitive_concepts(states);
    generate_bot_concept(states);
    generate_top_concept(states);
    generate_primitive_roles(states);
    generate_top_role(states);
    generate_one_of_concept(states);
    utils::g_log << "Complexity " << 1 << ":" << std::endl;
    print_brief_statistics();
    utils::g_log << "Finished generating base features." << std::endl;
}

void FeatureGeneratorImpl::generate_inductively(const States& states, FeatureRepresentations& feature_repr) {
    utils::g_log << "Started generating composite features." << std::endl;
    for (int iteration = 1; iteration < m_complexity; ++iteration) {  // every composition adds at least one complexity
        if (reached_limit()) break;
        if (m_generate_empty_boolean) generate_empty_boolean(states, iteration, feature_repr);
        if (m_generate_all_concept) generate_all_concept(states, iteration);
        if (m_generate_and_concept) generate_and_concept(states, iteration);
        if (m_generate_diff_concept) generate_diff_concept(states, iteration);
        if (m_generate_equal_concept) generate_equal_concept(states, iteration);
        if (m_generate_not_concept) generate_not_concept(states, iteration);
        if (m_generate_or_concept) generate_or_concept(states, iteration);
        if (m_generate_projection_concept) generate_projection_concept(states, iteration);
        if (m_generate_some_concept) generate_some_concept(states, iteration);
        if (m_generate_subset_concept) generate_subset_concept(states, iteration);
        if (m_generate_concept_distance_numerical) generate_concept_distance_numerical(states, iteration, feature_repr);
        if (m_generate_count_numerical) generate_count_numerical(states, iteration, feature_repr);
        if (m_generate_role_distance_numerical) generate_role_distance_numerical(states, iteration, feature_repr);
        if (m_generate_sum_concept_distance_numerical) generate_sum_concept_distance_numerical(states, iteration, feature_repr);
        if (m_generate_sum_role_distance_numerical) generate_sum_role_distance_numerical(states, iteration, feature_repr);
        if (m_generate_and_role) generate_and_role(states, iteration);
        if (m_generate_compose_role) generate_compose_role(states, iteration);
        if (m_generate_diff_role) generate_diff_role(states, iteration);
        if (m_generate_identity_role) generate_identity_role(states, iteration);
        if (m_generate_inverse_role) generate_inverse_role(states, iteration);
        if (m_generate_not_role) generate_not_role(states, iteration);
        if (m_generate_or_role) generate_or_role(states, iteration);
        if (m_generate_restrict_role) generate_restrict_role(states, iteration);
        if (m_generate_transitive_closure_role) generate_transitive_closure_role(states, iteration);
        if (m_generate_transitive_reflexive_closure_role) generate_transitive_reflexive_closure_role(states, iteration);

        utils::g_log << "Complexity " << iteration+1 << ":" << std::endl;
        print_brief_statistics();
    }
    utils::g_log << "Finished generating composite features." << std::endl;
}

bool FeatureGeneratorImpl::reached_limit() const {
    return (m_timer.is_expired() || (m_count_features >= m_feature_limit));
}

void FeatureGeneratorImpl::add_concept(const States& states, core::Concept&& concept) {
    const std::vector<int>& denotation = flatten_concept_denotation(evaluate<core::ConceptDenotation>(concept, states));
    if (m_hash_table->insert_concept(denotation)) {
        m_concept_elements_by_complexity[concept.compute_complexity()].emplace_back(concept);
    }
}

void FeatureGeneratorImpl::add_role(const States& states, core::Role&& role) {
    const std::vector<int>& denotation = flatten_role_denotation(evaluate<core::RoleDenotation>(role, states));
    if (m_hash_table->insert_role(denotation)) {
        m_role_elements_by_complexity[role.compute_complexity()].emplace_back(role);
    }
}

void FeatureGeneratorImpl::add_numerical(const States& states, core::Numerical&& numerical, FeatureRepresentations& feature_reprs) {
    std::string repr = numerical.compute_repr();
    const std::vector<int>& denotation = evaluate<int>(numerical, states);
    if (m_hash_table->insert_numerical(denotation)) {
        m_numerical_elements_by_complexity[numerical.compute_complexity()].emplace_back(numerical);
        feature_reprs.push_back(repr);
        ++m_count_features;
    }
}

void FeatureGeneratorImpl::add_boolean(const States& states, core::Boolean&& boolean, FeatureRepresentations& feature_reprs) {
    std::string repr = boolean.compute_repr();
    const std::vector<bool>& denotation = evaluate<bool>(boolean, states);
    if (m_hash_table->insert_boolean(denotation)) {
        m_boolean_elements_by_complexity[boolean.compute_complexity()].emplace_back(boolean);
        feature_reprs.push_back(repr);
        ++m_count_features;
    }
}

void FeatureGeneratorImpl::generate_primitive_concepts(const States& states) {
    const std::vector<core::Predicate>& predicates = m_factory->get_vocabulary_info()->get_predicates();
    for (const auto& predicate : predicates) {
        for (int pos = 0; pos < predicate.get_arity(); ++pos) {
            add_concept(states, m_factory->make_primitive_concept(predicate, pos));
        }
    }
}

void FeatureGeneratorImpl::generate_primitive_roles(const States& states) {
    const std::vector<core::Predicate>& predicates = m_factory->get_vocabulary_info()->get_predicates();
    for (const auto& predicate : predicates) {
        for (int pos1 = 0; pos1 < predicate.get_arity(); ++pos1) {
            for (int pos2 = pos1 + 1; pos2 < predicate.get_arity(); ++pos2) {
                add_role(states, m_factory->make_primitive_role(predicate, pos1, pos2));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_top_role(const States& states) {
    add_role(states, m_factory->make_top_role());
}

void FeatureGeneratorImpl::generate_bot_concept(const States& states) {
    add_concept(states, m_factory->make_bot_concept());
}

void FeatureGeneratorImpl::generate_top_concept(const States& states) {
    add_concept(states, m_factory->make_top_concept());
}

void FeatureGeneratorImpl::generate_one_of_concept(const States& states) {
    for (const auto& constant : m_factory->get_vocabulary_info()->get_constants()) {
        add_concept(states, m_factory->make_one_of_concept(constant));
    }
}


void FeatureGeneratorImpl::generate_empty_boolean(const States& states, int iteration, FeatureRepresentations& feature_reprs) {
    for (const auto& concept : m_concept_elements_by_complexity[iteration]) {
        if (reached_limit()) return;
        else add_boolean(states, m_factory->make_empty_boolean(concept), feature_reprs);
    }
    for (const auto& role : m_role_elements_by_complexity[iteration]) {
        if (reached_limit()) return;
        else add_boolean(states, m_factory->make_empty_boolean(role), feature_reprs);
    }
}

void FeatureGeneratorImpl::generate_all_concept(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        for (const auto& role : m_role_elements_by_complexity[i]) {
            for (const auto& concept : m_concept_elements_by_complexity[j]) {
                if (reached_limit()) return;
                else add_concept(states, m_factory->make_all_concept(role, concept));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_and_concept(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        for (const auto& concept_left : m_concept_elements_by_complexity[i]) {
            for (const auto& concept_right : m_concept_elements_by_complexity[j]) {
                if (reached_limit()) return;
                else add_concept(states, m_factory->make_and_concept(concept_left, concept_right));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_diff_concept(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        for (const auto& concept_left : m_concept_elements_by_complexity[i]) {
            for (const auto& concept_right : m_concept_elements_by_complexity[j]) {
                if (reached_limit()) return;
                else add_concept(states, m_factory->make_diff_concept(concept_left, concept_right));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_equal_concept(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        for (const auto& role_left : m_role_elements_by_complexity[i]) {
            for (const auto& role_right : m_role_elements_by_complexity[j]) {
                if (reached_limit()) return;
                else add_concept(states, m_factory->make_equal_concept(role_left, role_right));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_not_concept(const States& states, int iteration) {
    for (const auto& concept : m_concept_elements_by_complexity[iteration]) {
        if (reached_limit()) return;
        else add_concept(states, m_factory->make_not_concept(concept));
    }
}

void FeatureGeneratorImpl::generate_or_concept(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        for (const auto& concept_left : m_concept_elements_by_complexity[i]) {
            for (const auto& concept_right : m_concept_elements_by_complexity[j]) {
                if (reached_limit()) return;
                else add_concept(states, m_factory->make_or_concept(concept_left, concept_right));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_projection_concept(const States& states, int iteration) {
    for (const auto& role : m_role_elements_by_complexity[iteration]) {
        if (!m_timer.is_expired()) {
            for (int pos = 0; pos < 2; ++pos) {
                if (reached_limit()) return;
                else add_concept(states, m_factory->make_projection_concept(role, pos));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_some_concept(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        for (const auto& role : m_role_elements_by_complexity[i]) {
            for (const auto& concept : m_concept_elements_by_complexity[j]) {
                if (reached_limit()) return;
                else add_concept(states, m_factory->make_some_concept(role, concept));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_subset_concept(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        for (const auto& role_left : m_role_elements_by_complexity[i]) {
            for (const auto& role_right : m_role_elements_by_complexity[j]) {
                if (reached_limit()) return;
                else add_concept(states, m_factory->make_subset_concept(role_left, role_right));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_concept_distance_numerical(const States& states, int iteration, FeatureRepresentations& feature_reprs) {
    for (int i = 1; i < iteration; ++i) {
        for (int j = 1; j < iteration - i; ++j) {
            int k = iteration - i - j;
            for (const auto& concept_left : m_concept_elements_by_complexity[i]) {
                for (const auto& role : m_role_elements_by_complexity[j]) {
                    for (const auto& concept_right : m_concept_elements_by_complexity[k]) {
                        if (reached_limit()) return;
                        else add_numerical(states, m_factory->make_concept_distance(concept_left, role, concept_right), feature_reprs);
                    }
                }
            }
        }
    }
}

void FeatureGeneratorImpl::generate_count_numerical(const States& states, int iteration, FeatureRepresentations& feature_reprs) {
    for (const auto& concept : m_concept_elements_by_complexity[iteration]) {
        if (reached_limit()) return;
        else add_numerical(states, m_factory->make_count(concept), feature_reprs);
    }
    for (const auto& role : m_role_elements_by_complexity[iteration]) {
        if (reached_limit()) return;
        else add_numerical(states, m_factory->make_count(role), feature_reprs);
    }
}

void FeatureGeneratorImpl::generate_role_distance_numerical(const States& states, int iteration, FeatureRepresentations& feature_reprs) {
    for (int i = 1; i < iteration; ++i) {
        for (int j = 1; j < iteration - i; ++j) {
            int k = iteration - i - j;
            for (const auto& role_left : m_role_elements_by_complexity[i]) {
                for (const auto& role : m_role_elements_by_complexity[j]) {
                    for (const auto& role_right : m_role_elements_by_complexity[k]) {
                        if (reached_limit()) return;
                        else add_numerical(states, m_factory->make_role_distance(role_left, role, role_right), feature_reprs);
                    }
                }
            }
        }
    }
}

void FeatureGeneratorImpl::generate_sum_concept_distance_numerical(const States& states, int iteration, FeatureRepresentations& feature_reprs) {
    for (int i = 1; i < iteration; ++i) {
        for (int j = 1; j < iteration - i; ++j) {
            int k = iteration - i - j;
            for (const auto& concept_left : m_concept_elements_by_complexity[i]) {
                for (const auto& role : m_role_elements_by_complexity[j]) {
                    for (const auto& concept_right : m_concept_elements_by_complexity[k]) {
                        if (reached_limit()) return;
                        else add_numerical(states, m_factory->make_sum_concept_distance(concept_left, role, concept_right), feature_reprs);
                    }
                }
            }
        }
    }
}

void FeatureGeneratorImpl::generate_sum_role_distance_numerical(const States& states, int iteration, FeatureRepresentations& feature_reprs) {
    for (int i = 1; i < iteration; ++i) {
        for (int j = 1; j < iteration - i; ++j) {
            int k = iteration - i - j;
            for (const auto& role_left : m_role_elements_by_complexity[i]) {
                for (const auto& role : m_role_elements_by_complexity[j]) {
                    for (const auto& role_right : m_role_elements_by_complexity[k]) {
                        if (reached_limit()) return;
                        else add_numerical(states, m_factory->make_sum_role_distance(role_left, role, role_right), feature_reprs);
                    }
                }
            }
        }
    }
}

void FeatureGeneratorImpl::generate_and_role(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        for (const auto& role_left : m_role_elements_by_complexity[i]) {
            for (const auto& role_right : m_role_elements_by_complexity[j]) {
                if (reached_limit()) return;
                else add_role(states, m_factory->make_and_role(role_left, role_right));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_compose_role(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        for (const auto& role_left : m_role_elements_by_complexity[i]) {
            for (const auto& role_right : m_role_elements_by_complexity[j]) {
                if (reached_limit()) return;
                else add_role(states, m_factory->make_compose_role(role_left, role_right));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_diff_role(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        for (const auto& role_left : m_role_elements_by_complexity[i]) {
            for (const auto& role_right : m_role_elements_by_complexity[j]) {
                if (reached_limit()) return;
                else add_role(states, m_factory->make_diff_role(role_left, role_right));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_identity_role(const States& states, int iteration) {
    for (const auto& concept : m_concept_elements_by_complexity[iteration]) {
        if (reached_limit()) return;
        else add_role(states, m_factory->make_identity_role(concept));
    }
}

void FeatureGeneratorImpl::generate_inverse_role(const States& states, int iteration) {
    for (const auto& role : m_role_elements_by_complexity[iteration]) {
        if (reached_limit()) return;
        else add_role(states, m_factory->make_inverse_role(role));
    }
}

void FeatureGeneratorImpl::generate_not_role(const States& states, int iteration) {
    for (const auto& role : m_role_elements_by_complexity[iteration]) {
        if (reached_limit()) return;
        else add_role(states, m_factory->make_not_role(role));
    }
}

void FeatureGeneratorImpl::generate_or_role(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        for (const auto& role_left : m_role_elements_by_complexity[i]) {
            for (const auto& role_right : m_role_elements_by_complexity[j]) {
                if (reached_limit()) return;
                else add_role(states, m_factory->make_or_role(role_left, role_right));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_restrict_role(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        for (const auto& role : m_role_elements_by_complexity[i]) {
            for (const auto& concept : m_concept_elements_by_complexity[j]) {
                if (reached_limit()) return;
                else add_role(states, m_factory->make_restrict_role(role, concept));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_transitive_closure_role(const States& states, int iteration) {
    for (const auto& role : m_role_elements_by_complexity[iteration]) {
        if (reached_limit()) return;
        else add_role(states, m_factory->make_transitive_closure(role));
    }
}

void FeatureGeneratorImpl::generate_transitive_reflexive_closure_role(const States& states, int iteration) {
    for (const auto& role : m_role_elements_by_complexity[iteration]) {
        if (reached_limit()) return;
        else add_role(states, m_factory->make_transitive_reflexive_closure(role));
    }
}

void FeatureGeneratorImpl::print_brief_statistics() const {
    std::cout << "Total generated features: " << m_hash_table->get_cache_hits() + m_hash_table->get_cache_misses() << std::endl
              << "Total novel features: " << m_hash_table->get_cache_misses() << std::endl
              << "Total concept elements: " << num_elements(m_concept_elements_by_complexity) << std::endl
              << "Total role elements: " << num_elements(m_role_elements_by_complexity) << std::endl
              << "Total numerical elements: " << num_elements(m_numerical_elements_by_complexity) << std::endl
              << "Total boolean elements: " << num_elements(m_boolean_elements_by_complexity) << std::endl;
}

void FeatureGeneratorImpl::print_overall_statistics() const {
    std::cout << "Generated concepts (complexity representation):" << std::endl;
    print_elements(m_concept_elements_by_complexity);
    std::cout << "Generated roles (complexity representation):" << std::endl;
    print_elements(m_role_elements_by_complexity);
    std::cout << "Generated numericals (complexity representation):" << std::endl;
    print_elements(m_numerical_elements_by_complexity);
    std::cout << "Generated booleans (complexity representation):" << std::endl;
    print_elements(m_boolean_elements_by_complexity);
}

}
