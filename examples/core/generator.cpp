#include "../../include/dlp/core.h"

#include <iostream>
#include <unordered_set>
#include <numeric>


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
 * An example that illustrates using the core component in the generator component.
 */
int main() {
    // 0. Initialize the vocabulary
    dlp::core::VocabularyInfo vocabulary;
    dlp::core::Predicate p0 = vocabulary.add_predicate("on", 2);
    dlp::core::Predicate p1 = vocabulary.add_predicate("onTable", 1);
    dlp::core::Predicate p2 = vocabulary.add_predicate("holding", 1);
    dlp::core::Predicate p3 = vocabulary.add_predicate("on_g", 2);
    std::vector<dlp::core::Predicate> predicates({p0, p1, p2, p3});

    // 1. Initialize planning instance
    dlp::core::InstanceInfo instance(vocabulary);
    // Add state atoms
    dlp::core::Atom a0 = instance.add_atom("on", {"A", "B"});
    dlp::core::Atom a1 = instance.add_atom("on", {"B", "A"});
    dlp::core::Atom a2 = instance.add_atom("onTable", {"A"});
    dlp::core::Atom a3 = instance.add_atom("onTable", {"B"});
    dlp::core::Atom a4 = instance.add_atom("holding", {"A"});
    dlp::core::Atom a5 = instance.add_atom("holding", {"B"});
    // Add goal atoms
    dlp::core::Atom a6 = instance.add_static_atom("on_g", {"A", "B"});

    // 2. Initialize factory.
    dlp::core::SyntacticElementFactory factory(vocabulary);

    // 3. Initialize containers for storage and uniqueness checking.
    // Equivalence checking using repr.
    std::unordered_set<std::string> concept_element_cache;
    std::unordered_set<std::string> role_element_cache;
    std::unordered_set<std::string> numerical_element_cache;
    std::unordered_set<std::string> boolean_element_cache;
    // Constructed elements sorted by complexity for incremental construction.
    int complexity_bound = 6;
    std::vector<std::vector<dlp::core::Concept>> concept_elements_by_complexity(complexity_bound);
    std::vector<std::vector<dlp::core::Role>> role_elements_by_complexity(complexity_bound);
    std::vector<std::vector<dlp::core::Numerical>> numerical_elements_by_complexity(complexity_bound);
    std::vector<std::vector<dlp::core::Boolean>> boolean_elements_by_complexity(complexity_bound);

    // 4. Construct base
    for (const auto& predicate: predicates) {
        // 4.1. PrimitiveConcept
        for (unsigned pos = 0; pos < predicate.get_arity(); ++pos) {
            dlp::core::Concept concept_element = factory.make_primitive_concept_element(predicate.get_name(), pos);
            bool unique = concept_element_cache.insert(concept_element.compute_repr()).second;
            if (unique) concept_elements_by_complexity[0].emplace_back(concept_element);
        }
        // 4.2. PrimitiveRole
        for (unsigned pos1 = 0; pos1 < predicate.get_arity(); ++pos1) {
            for (unsigned pos2 = 0; pos2 < predicate.get_arity(); ++pos2) {
                // TODO(dominik): add roles
                dlp::core::Role role_element = factory.make_primitive_role_element(predicate.get_name(), pos1, pos2);
                bool unique = role_element_cache.insert(role_element.compute_repr()).second;
                if (unique) role_elements_by_complexity[0].emplace_back(role_element);

            }
        }
        // TODO(dominik): Add other complexity 1 elements, e.g. top, bot,..
    }
    // 5. Interatively construct more complex elements
    for (int iteration = 0; iteration < complexity_bound; ++iteration) {
        for (int i = 0; i <= iteration; ++i) {
            // 5.1. AndConcept:
            for (const auto& c1 : concept_elements_by_complexity[i]) {
                for (const auto& c2 : concept_elements_by_complexity[iteration]) {
                    if (c1.compute_repr() == c2.compute_repr()) continue;
                    dlp::core::Concept concept_element = factory.make_and_concept_element(c1, c2);
                    bool unique = concept_element_cache.insert(concept_element.compute_repr()).second;
                    if (unique && concept_element.compute_complexity() < complexity_bound) {
                        concept_elements_by_complexity[concept_element.compute_complexity()].emplace_back(concept_element);
                    }
                }
            }
            // 5.2. OrConceptElement:
            // 5.3. ...
        }
    }

    std::cout << "Total concept elements: " << num_elements(concept_elements_by_complexity) << std::endl;
    print_elements(concept_elements_by_complexity);
    std::cout << "Total role elements: " << num_elements(role_elements_by_complexity) << std::endl;
    print_elements(role_elements_by_complexity);
    std::cout << "Total numerical elements: " << num_elements(numerical_elements_by_complexity) << std::endl;
    std::cout << "Total boolean elements: " << num_elements(boolean_elements_by_complexity) << std::endl;
};
