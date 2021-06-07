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
      m_concept_elements_by_complexity(complexity),
      m_role_elements_by_complexity(complexity),
      m_numerical_elements_by_complexity(complexity),
      m_boolean_elements_by_complexity(complexity) { }

FeatureCollection FeatureGeneratorImpl::generate(const States& states) {
    FeatureCollection feature_collection;

    generate_base(states);
    generate_inductively(states);

    return feature_collection;
}


void FeatureGeneratorImpl::generate_base(const States& states) {
    generate_primitive_concepts(states);
    generate_primitive_roles(states);

    std::cout << "Total concept elements: " << num_elements(m_concept_elements_by_complexity) << std::endl;
    print_elements(m_concept_elements_by_complexity);
    std::cout << "Total role elements: " << num_elements(m_role_elements_by_complexity) << std::endl;
    print_elements(m_role_elements_by_complexity);
    std::cout << "Total numerical elements: " << num_elements(m_numerical_elements_by_complexity) << std::endl;
    std::cout << "Total boolean elements: " << num_elements(m_boolean_elements_by_complexity) << std::endl;
}

void FeatureGeneratorImpl::generate_primitive_concepts(const States& states) {
    const std::vector<core::Predicate>& predicates = m_factory->get_vocabulary_info()->get_predicates();
    for (const auto& predicate : predicates) {
        for (unsigned pos = 0; pos < predicate.get_arity(); ++pos) {
            dlp::core::Concept concept_element = m_factory->make_primitive_concept(predicate.get_name(), pos);
            bool syntactically_unique = m_concept_element_cache.insert(concept_element.compute_repr()).second;
            bool empirically_unique = m_concept_denotation_cache.insert(evaluate<core::ConceptDenotation>(concept_element, states)).second;
            if (syntactically_unique && empirically_unique) m_concept_elements_by_complexity[0].emplace_back(concept_element);
        }
    }
}

void FeatureGeneratorImpl::generate_primitive_roles(const States& states) {
    const std::vector<core::Predicate>& predicates = m_factory->get_vocabulary_info()->get_predicates();
    for (const auto& predicate : predicates) {
        for (unsigned pos1 = 0; pos1 < predicate.get_arity(); ++pos1) {
            for (unsigned pos2 = 0; pos2 < predicate.get_arity(); ++pos2) {
                dlp::core::Role role_element = m_factory->make_primitive_role(predicate.get_name(), pos1, pos2);
                bool syntactically_unique = m_role_element_cache.insert(role_element.compute_repr()).second;
                bool empirically_unique = m_role_denotation_cache.insert(evaluate<core::RoleDenotation>(role_element, states)).second;
                if (syntactically_unique && empirically_unique) m_role_elements_by_complexity[0].emplace_back(role_element);
            }
        }
    }
}

void FeatureGeneratorImpl::generate_top_role() {

}

void FeatureGeneratorImpl::generate_bot_concept() {

}

void FeatureGeneratorImpl::generate_top_concept() {

}


void FeatureGeneratorImpl::generate_inductively(const States& states) {
}

}
}
