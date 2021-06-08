#include "feature_generator.h"

#include <iostream>

#include "../../include/dlp/generator.h"


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
    generate_inductively(states);
    print_statistics();
    return feature_collection;
}


void FeatureGeneratorImpl::generate_base(const States& states) {
    generate_primitive_concepts(states);
    generate_bot_concept(states);
    generate_top_concept(states);
    generate_primitive_roles(states);
    generate_top_role(states);
}

void FeatureGeneratorImpl::generate_inductively(const States& states) {
    for (int iteration = 1; iteration <= m_complexity; ++iteration) {
        generate_empty_boolean(states, iteration);
        generate_all_concept(states, iteration);
        generate_and_concept(states, iteration);
        generate_or_concept(states, iteration);
    }
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

void FeatureGeneratorImpl::add_numerical(const States& states, core::Numerical&& numerical) {
    bool syntactically_unique = m_numerical_element_cache.insert(numerical.compute_repr()).second;
    bool empirically_unique = m_numerical_denotation_cache.insert(evaluate<int>(numerical, states)).second;
    if (syntactically_unique && empirically_unique) {
        m_numerical_elements_by_complexity[numerical.compute_complexity()].emplace_back(numerical);
        ++m_cache_misses;
    } else {
        ++m_cache_hits;
    }
}

void FeatureGeneratorImpl::add_boolean(const States& states, core::Boolean&& boolean) {
    bool syntactically_unique = m_boolean_element_cache.insert(boolean.compute_repr()).second;
    bool empirically_unique = m_boolean_denotation_cache.insert(evaluate<bool>(boolean, states)).second;
    if (syntactically_unique && empirically_unique) {
        m_boolean_elements_by_complexity[boolean.compute_complexity()].emplace_back(boolean);
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


void FeatureGeneratorImpl::generate_empty_boolean(const States& states, int iteration) {
    for (const auto& concept : m_concept_elements_by_complexity[iteration]) {
        add_boolean(states, m_factory->make_empty_boolean(concept));
    }
    for (const auto& role : m_role_elements_by_complexity[iteration]) {
        add_boolean(states, m_factory->make_empty_boolean(role));
    }
}

void FeatureGeneratorImpl::generate_all_concept(const States& states, int iteration) {
    for (int i = 1; i < m_complexity - iteration; ++i) {
        for (const auto& role : m_role_elements_by_complexity[iteration]) {
            for (const auto& concept : m_concept_elements_by_complexity[i]) {
                add_concept(states, m_factory->make_all_concept(role, concept));
            }
        }
        // exchanged iteration bounds.
        for (const auto& role : m_role_elements_by_complexity[i]) {
            for (const auto& concept : m_concept_elements_by_complexity[iteration]) {
                add_concept(states, m_factory->make_all_concept(role, concept));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_and_concept(const States& states, int iteration) {
    for (int i = 1; i < m_complexity - iteration; ++i) {
        for (const auto& concept_left : m_concept_elements_by_complexity[iteration]) {
            for (const auto& concept_right : m_concept_elements_by_complexity[i]) {
                add_concept(states, m_factory->make_and_concept(concept_left, concept_right));
            }
        }
    }
}

void FeatureGeneratorImpl::generate_or_concept(const States& states, int iteration) {
    for (int i = 1; i < m_complexity - iteration; ++i) {
        for (const auto& concept_left : m_concept_elements_by_complexity[iteration]) {
            for (const auto& concept_right : m_concept_elements_by_complexity[i]) {
                add_concept(states, m_factory->make_or_concept(concept_left, concept_right));
            }
        }
    }
}

void FeatureGeneratorImpl::print_statistics() const {
    std::cout << "=================================================================" << std::endl
              << "=====                Overview:                              =====" << std::endl
              << "=================================================================" << std::endl
              << "Cache hits: " << m_cache_hits << std::endl
              << "Cache misses: " << m_cache_misses << std::endl
              << "Total concept elements: " << num_elements(m_concept_elements_by_complexity) << std::endl
              << "Total role elements: " << num_elements(m_role_elements_by_complexity) << std::endl
              << "Total numerical elements: " << num_elements(m_numerical_elements_by_complexity) << std::endl
              << "Total boolean elements: " << num_elements(m_boolean_elements_by_complexity) << std::endl
              << "=================================================================" << std::endl;

    print_elements(m_concept_elements_by_complexity);
    print_elements(m_role_elements_by_complexity);
}

}
}
