#include "feature_generator.h"

#include <algorithm>
#include <iostream>
#include <cassert>

#include "../../include/dlp/generator.h"
#include "../utils/logging.h"
#include "boolean.h"
#include "numerical.h"


namespace dlp {
namespace generator {

template<typename T>
static unsigned num_elements(const std::vector<std::vector<T>>& elements_by_complexity) {
    unsigned count = 0;
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
template<typename T>
static std::vector<T> evaluate(core::Element<T>& element, const States& states) {
    std::vector<T> result;
    result.reserve(states.size());
    for (const auto& state : states) {
        result.push_back(element.evaluate(state));
    }
    return result;
}


FeatureGeneratorImpl::FeatureGeneratorImpl(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit)
    : m_factory(factory), m_complexity(complexity), m_time_limit(time_limit),
      m_concept_elements_by_complexity(complexity+1),
      m_role_elements_by_complexity(complexity+1),
      m_numerical_elements_by_complexity(complexity+1),
      m_boolean_elements_by_complexity(complexity+1),
      m_cache_misses(0),
      m_cache_hits(0) { }

FeatureCollection FeatureGeneratorImpl::generate(const States& states) {
    FeatureCollection feature_collection;
    generate_base(states);
    generate_inductively(states, feature_collection);
    utils::g_log << "Overall results: " << std::endl;
    print_overall_statistics();
    return feature_collection;
}


void FeatureGeneratorImpl::generate_base(const States& states) {
    utils::g_log << "Started generating base features of complexity 1." << std::endl;
    generate_primitive_concepts(states);
    generate_bot_concept(states);
    generate_top_concept(states);
    generate_primitive_roles(states);
    generate_top_role(states);
    utils::g_log << "Complexity " << 1 << ":" << std::endl;
        print_brief_statistics();
    utils::g_log << "Finished generating base features." << std::endl;
}

void FeatureGeneratorImpl::generate_inductively(const States& states, FeatureCollection& feature_collection) {
    utils::g_log << "Started generating composite features." << std::endl;
    for (int iteration = 1; iteration < m_complexity; ++iteration) {  // every composition adds at least one complexity
        generate_empty_boolean(states, iteration, feature_collection);
        generate_all_concept(states, iteration);
        generate_and_concept(states, iteration);
        generate_diff_concept(states, iteration);
        generate_not_concept(states, iteration);
        generate_or_concept(states, iteration);
        generate_some_concept(states, iteration);
        generate_subset_concept(states, iteration);
        generate_concept_distance_numerical(states, iteration, feature_collection);
        generate_count_numerical(states, iteration, feature_collection);
        generate_role_distance_numerical(states, iteration, feature_collection);
        //generate_sum_concept_distance_numerical(states, iteration, feature_collection);
        //generate_sum_role_distance_numerical(states, iteration, feature_collection);
        generate_and_role(states, iteration);
        generate_compose_role(states, iteration);
        generate_diff_role(states, iteration);
        generate_identity_role(states, iteration);
        generate_inverse_role(states, iteration);
        generate_not_role(states, iteration);
        generate_or_role(states, iteration);
        generate_restrict_role(states, iteration);
        generate_transitive_closure_role(states, iteration);
        generate_transitive_reflexive_closure_role(states, iteration);

        utils::g_log << "Complexity " << iteration+1 << ":" << std::endl;
        print_brief_statistics();
    }
    utils::g_log << "Finished generating composite features." << std::endl;
}

void FeatureGeneratorImpl::add_concept(const States& states, core::Concept&& concept) {
    bool syntactically_unique = m_concept_element_cache.insert(concept.compute_repr()).second;
    bool empirically_unique = m_concept_denotation_cache.insert(evaluate<core::ConceptDenotation>(concept, states)).second;
    if (syntactically_unique && empirically_unique) {
        m_concept_elements_by_complexity[concept.compute_complexity()].emplace_back(concept);
        ++m_cache_misses;
    } else {
        ++m_cache_hits;
    }
}

void FeatureGeneratorImpl::add_role(const States& states, core::Role&& role) {
    bool syntactically_unique = m_role_element_cache.insert(role.compute_repr()).second;
    bool empirically_unique = m_role_denotation_cache.insert(evaluate<core::RoleDenotation>(role, states)).second;
    if (syntactically_unique && empirically_unique) {
        m_role_elements_by_complexity[role.compute_complexity()].emplace_back(role);
        ++m_cache_misses;
    } else {
        ++m_cache_hits;
    }
}

void FeatureGeneratorImpl::add_numerical(const States& states, core::Numerical&& numerical, FeatureCollection& feature_collection) {
    std::string repr = numerical.compute_repr();
    bool syntactically_unique = m_numerical_element_cache.insert(repr).second;
    std::vector<int> denotation = evaluate<int>(numerical, states);
    bool empirically_unique = m_numerical_denotation_cache.insert(denotation).second;
    if (syntactically_unique && empirically_unique) {
        m_numerical_elements_by_complexity[numerical.compute_complexity()].emplace_back(numerical);
        feature_collection.add_numerical_feature(Numerical(repr, denotation));
        ++m_cache_misses;
    } else {
        ++m_cache_hits;
    }
}

void FeatureGeneratorImpl::add_boolean(const States& states, core::Boolean&& boolean, FeatureCollection& feature_collection) {
    std::string repr = boolean.compute_repr();
    bool syntactically_unique = m_boolean_element_cache.insert(repr).second;
    std::vector<bool> denotation = evaluate<bool>(boolean, states);
    bool empirically_unique = m_boolean_denotation_cache.insert(denotation).second;
    if (syntactically_unique && empirically_unique) {
        m_boolean_elements_by_complexity[boolean.compute_complexity()].emplace_back(boolean);
        feature_collection.add_boolean_feature(Boolean(repr, denotation));
        ++m_cache_misses;
    } else {
        ++m_cache_hits;
    }
}

void FeatureGeneratorImpl::generate_primitive_concepts(const States& states) {
    const std::vector<core::Predicate>& predicates = m_factory->get_vocabulary_info()->get_predicates();
    for (const auto& predicate : predicates) {
        for (unsigned pos = 0; pos < predicate.get_arity(); ++pos) {
            add_concept(states, m_factory->make_primitive_concept(predicate.get_name(), pos));
        }
    }
}

void FeatureGeneratorImpl::generate_primitive_roles(const States& states) {
    const std::vector<core::Predicate>& predicates = m_factory->get_vocabulary_info()->get_predicates();
    for (const auto& predicate : predicates) {
        for (unsigned pos1 = 0; pos1 < predicate.get_arity(); ++pos1) {
            for (unsigned pos2 = 0; pos2 < predicate.get_arity(); ++pos2) {
                add_role(states, m_factory->make_primitive_role(predicate.get_name(), pos1, pos2));
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


void FeatureGeneratorImpl::generate_empty_boolean(const States& states, int iteration, FeatureCollection& feature_collection) {
    for (const auto& concept : m_concept_elements_by_complexity[iteration]) {
        add_boolean(states, m_factory->make_empty_boolean(concept), feature_collection);
    }
    for (const auto& role : m_role_elements_by_complexity[iteration]) {
        add_boolean(states, m_factory->make_empty_boolean(role), feature_collection);
    }
}

void FeatureGeneratorImpl::generate_all_concept(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        assert(i + j == iteration);
        int indices[2] = {i, j};
        std::sort(indices, indices+2);
        do {
            for (const auto& role : m_role_elements_by_complexity[indices[0]]) {
                for (const auto& concept : m_concept_elements_by_complexity[indices[1]]) {
                    add_concept(states, m_factory->make_all_concept(role, concept));
                }
            }
        } while (std::next_permutation(indices, indices+2));
    }
}

void FeatureGeneratorImpl::generate_and_concept(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        assert(i + j == iteration);
        for (const auto& concept_left : m_concept_elements_by_complexity[i]) {
            for (const auto& concept_right : m_concept_elements_by_complexity[j]) {
                add_concept(states, m_factory->make_and_concept(concept_left, concept_right));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_diff_concept(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        assert(i + j == iteration);
        int indices[2] = {i, j};
        std::sort(indices, indices+2);
        do {
            for (const auto& concept_left : m_concept_elements_by_complexity[indices[0]]) {
                for (const auto& concept_right : m_concept_elements_by_complexity[indices[1]]) {
                    add_concept(states, m_factory->make_diff_concept(concept_left, concept_right));
                }
            }
        } while (std::next_permutation(indices, indices+2));
    }
}

void FeatureGeneratorImpl::generate_not_concept(const States& states, int iteration) {
    for (const auto& concept : m_concept_elements_by_complexity[iteration]) {
        add_concept(states, m_factory->make_not_concept(concept));
    }
}

void FeatureGeneratorImpl::generate_one_of_concept(const States& states, int iteration) {
    throw std::runtime_error("FeatureGeneratorImpl::generate_one_of_concept - not implemented");
}


void FeatureGeneratorImpl::generate_or_concept(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        assert(i + j == iteration);
        for (const auto& concept_left : m_concept_elements_by_complexity[i]) {
            for (const auto& concept_right : m_concept_elements_by_complexity[j]) {
                add_concept(states, m_factory->make_or_concept(concept_left, concept_right));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_some_concept(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        assert(i + j == iteration);
        int indices[2] = {i, j};
        std::sort(indices, indices+2);
        do {
            for (const auto& role : m_role_elements_by_complexity[indices[0]]) {
                for (const auto& concept : m_concept_elements_by_complexity[indices[1]]) {
                    add_concept(states, m_factory->make_some_concept(role, concept));
                }
            }
        } while (std::next_permutation(indices, indices+2));
    }
}

void FeatureGeneratorImpl::generate_subset_concept(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        assert(i + j == iteration);
        int indices[2] = {i, j};
        std::sort(indices, indices+2);
        do {
            for (const auto& role_left : m_role_elements_by_complexity[indices[0]]) {
                for (const auto& role_right : m_role_elements_by_complexity[indices[1]]) {
                    add_concept(states, m_factory->make_subset_concept(role_left, role_right));
                }
            }
        } while (std::next_permutation(indices, indices+2));
    }
}

void FeatureGeneratorImpl::generate_concept_distance_numerical(const States& states, int iteration, FeatureCollection& feature_collection) {
    for (int i = 0; i < iteration; ++i) {
        for (int j = 0; j < iteration - i; ++j) {
            int k = iteration - i - j;
            assert(i + j + k == iteration);
            int indices[3] = {i, j, k};
            std::sort(indices, indices+3);
            do {
                for (const auto& concept_left : m_concept_elements_by_complexity[indices[0]]) {
                    for (const auto& role : m_role_elements_by_complexity[indices[1]]) {
                        for (const auto& concept_right : m_concept_elements_by_complexity[indices[2]]) {
                            add_numerical(states, m_factory->make_concept_distance(concept_left, role, concept_right), feature_collection);
                        }
                    }
                }
            } while (std::next_permutation(indices, indices+3));
        }
    }
}

void FeatureGeneratorImpl::generate_count_numerical(const States& states, int iteration, FeatureCollection& feature_collection) {
    for (const auto& concept : m_concept_elements_by_complexity[iteration]) {
        add_numerical(states, m_factory->make_count(concept), feature_collection);
    }
    for (const auto& role : m_role_elements_by_complexity[iteration]) {
        add_numerical(states, m_factory->make_count(role), feature_collection);
    }
}

void FeatureGeneratorImpl::generate_role_distance_numerical(const States& states, int iteration, FeatureCollection& feature_collection) {
    for (int i = 0; i < iteration; ++i) {
        for (int j = 0; j < iteration - i; ++j) {
            int k = iteration - i - j;
            assert(i + j + k == iteration);
            int indices[3] = {i, j, k};
            std::sort(indices, indices+3);
            do {
                for (const auto& role_left : m_role_elements_by_complexity[indices[0]]) {
                    for (const auto& role : m_role_elements_by_complexity[indices[1]]) {
                        for (const auto& role_right : m_role_elements_by_complexity[indices[2]]) {
                            add_numerical(states, m_factory->make_role_distance(role_left, role, role_right), feature_collection);
                        }
                    }
                }
            } while (std::next_permutation(indices, indices+3));
        }
    }
}

void FeatureGeneratorImpl::generate_sum_concept_distance_numerical(const States& states, int iteration, FeatureCollection& feature_collection) {
    for (int i = 0; i < iteration; ++i) {
        for (int j = 0; j < iteration - i; ++j) {
            int k = iteration - i - j;
            assert(i + j + k == iteration);
            int indices[3] = {i, j, k};
            std::sort(indices, indices+3);
            do {
                for (const auto& concept_left : m_concept_elements_by_complexity[indices[0]]) {
                    for (const auto& role : m_role_elements_by_complexity[indices[1]]) {
                        for (const auto& concept_right : m_concept_elements_by_complexity[indices[2]]) {
                            add_numerical(states, m_factory->make_sum_concept_distance(concept_left, role, concept_right), feature_collection);
                        }
                    }
                }
            } while (std::next_permutation(indices, indices+3));
        }
    }
}

void FeatureGeneratorImpl::generate_sum_role_distance_numerical(const States& states, int iteration, FeatureCollection& feature_collection) {
    for (int i = 0; i < iteration; ++i) {
        for (int j = 0; j < iteration - i; ++j) {
            int k = iteration - i - j;
            assert(i + j + k == iteration);
            int indices[3] = {i, j, k};
            std::sort(indices, indices+3);
            do {
                for (const auto& role_left : m_role_elements_by_complexity[indices[0]]) {
                    for (const auto& role : m_role_elements_by_complexity[indices[1]]) {
                        for (const auto& role_right : m_role_elements_by_complexity[indices[2]]) {
                            add_numerical(states, m_factory->make_sum_role_distance(role_left, role, role_right), feature_collection);
                        }
                    }
                }
            } while (std::next_permutation(indices, indices+3));
        }
    }
}

void FeatureGeneratorImpl::generate_and_role(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        assert(i + j == iteration);
        for (const auto& role_left : m_role_elements_by_complexity[i]) {
            for (const auto& role_right : m_role_elements_by_complexity[j]) {
                add_role(states, m_factory->make_and_role(role_left, role_right));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_compose_role(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        assert(i + j == iteration);
        int indices[2] = {i, j};
        std::sort(indices, indices+2);
        do {
            for (const auto& role_left : m_role_elements_by_complexity[indices[0]]) {
                for (const auto& role_right : m_role_elements_by_complexity[indices[1]]) {
                    add_role(states, m_factory->make_compose_role(role_left, role_right));
                }
            }
        } while (std::next_permutation(indices, indices+2));
    }
}

void FeatureGeneratorImpl::generate_diff_role(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        assert(i + j == iteration);
        int indices[2] = {i, j};
        std::sort(indices, indices+2);
        do {
            for (const auto& role_left : m_role_elements_by_complexity[indices[0]]) {
                for (const auto& role_right : m_role_elements_by_complexity[indices[1]]) {
                    add_role(states, m_factory->make_diff_role(role_left, role_right));
                }
            }
        } while (std::next_permutation(indices, indices+2));
    }
}

void FeatureGeneratorImpl::generate_identity_role(const States& states, int iteration) {
    for (const auto& concept : m_concept_elements_by_complexity[iteration]) {
        add_role(states, m_factory->make_identity_role(concept));
    }
}

void FeatureGeneratorImpl::generate_inverse_role(const States& states, int iteration) {
    for (const auto& role : m_role_elements_by_complexity[iteration]) {
        add_role(states, m_factory->make_inverse_role(role));
    }
}

void FeatureGeneratorImpl::generate_not_role(const States& states, int iteration) {
    for (const auto& role : m_role_elements_by_complexity[iteration]) {
        add_role(states, m_factory->make_not_role(role));
    }
}

void FeatureGeneratorImpl::generate_or_role(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        assert(i + j == iteration);
        int indices[2] = {i, j};
        do {
            for (const auto& role_left : m_role_elements_by_complexity[indices[0]]) {
                for (const auto& role_right : m_role_elements_by_complexity[indices[1]]) {
                    add_role(states, m_factory->make_or_role(role_left, role_right));
                }
            }
        } while (std::next_permutation(indices, indices+2));
    }
}

void FeatureGeneratorImpl::generate_restrict_role(const States& states, int iteration) {
    for (int i = 1; i < iteration; ++i) {
        int j = iteration - i;
        assert(i + j == iteration);
        int indices[2] = {i, j};
        do {
            for (const auto& role : m_role_elements_by_complexity[indices[0]]) {
                for (const auto& concept : m_concept_elements_by_complexity[indices[1]]) {
                    add_role(states, m_factory->make_restrict_role(role, concept));
                }
            }
        } while (std::next_permutation(indices, indices+2));
    }
}

void FeatureGeneratorImpl::generate_transitive_closure_role(const States& states, int iteration) {
    for (const auto& role : m_role_elements_by_complexity[iteration]) {
        add_role(states, m_factory->make_transitive_closure(role));
    }
}

void FeatureGeneratorImpl::generate_transitive_reflexive_closure_role(const States& states, int iteration) {
    for (const auto& role : m_role_elements_by_complexity[iteration]) {
        add_role(states, m_factory->make_transitive_reflexive_closure(role));
    }
}

void FeatureGeneratorImpl::print_brief_statistics() const {
    std::cout << "Total generated features: " << m_cache_hits + m_cache_misses << std::endl
              << "Total novel features: " << m_cache_misses << std::endl
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
}
