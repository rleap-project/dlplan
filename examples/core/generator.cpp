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
            std::cout << element.complexity() << " " << element.repr() << std::endl;
        }
    }
}


/**
 * An example that illustrates using the core component in the generator component.
 */
int main() {
    // 1. Initialize planning instance
    dlp::core::InstanceInfo instance;
    // Add state atoms
    dlp::core::Atom a0 = instance.add_atom("on", {"A", "B"}, false);
    dlp::core::Atom a1 = instance.add_atom("on", {"B", "A"}, false);
    dlp::core::Atom a2 = instance.add_atom("onTable", {"A"}, false);
    dlp::core::Atom a3 = instance.add_atom("onTable", {"B"}, false);
    dlp::core::Atom a4 = instance.add_atom("holding", {"A"}, false);
    dlp::core::Atom a5 = instance.add_atom("holding", {"B"}, false);
    // Add goal atoms
    dlp::core::Atom a6 = instance.add_atom("on_g", {"A", "B"}, true);

    // 2. Initialize factory.
    dlp::core::SyntacticElementFactory factory;

    // 3. Initialize containers for storage and uniqueness checking.
    // Equivalence checking using repr.
    std::unordered_set<std::string> concept_element_cache;
    std::unordered_set<std::string> role_element_cache;
    std::unordered_set<std::string> numerical_element_cache;
    std::unordered_set<std::string> boolean_element_cache;
    // Constructed elements sorted by complexity for incremental construction.
    int complexity_bound = 6;
    std::vector<std::vector<dlp::core::ConceptElement>> concept_elements_by_complexity(complexity_bound);
    std::vector<std::vector<dlp::core::RoleElement>> role_elements_by_complexity(complexity_bound);
    std::vector<std::vector<dlp::core::NumericalElement>> numerical_elements_by_complexity(complexity_bound);
    std::vector<std::vector<dlp::core::BooleanElement>> boolean_elements_by_complexity(complexity_bound);

    // 4. Construct base
    std::vector<dlp::core::Predicate> predicates = instance.predicates();
    for (const auto& predicate: predicates) {
        // 4.1. PrimitiveConceptElement
        for (unsigned pos = 0; pos < predicate.arity(); ++pos) {
            dlp::core::ConceptElement concept_element = factory.make_primitive_concept_element(predicate.name(), pos);
            bool unique = concept_element_cache.insert(concept_element.repr()).second;
            if (unique) concept_elements_by_complexity[0].emplace_back(concept_element);
        }
        // 4.2. PrimitiveRoleElement
        for (unsigned pos1 = 0; pos1 < predicate.arity(); ++pos1) {
            for (unsigned pos2 = 0; pos2 < predicate.arity(); ++pos2) {
                // TODO(dominik): add roles
                dlp::core::RoleElement role_element = factory.make_primitive_role_element(predicate.name(), pos1, pos2);
                bool unique = role_element_cache.insert(role_element.repr()).second;
                if (unique) role_elements_by_complexity[0].emplace_back(role_element);

            }
        }
        // TODO(dominik): Add other complexity 1 elements, e.g. top, bot,..
    }
    // 5. Interatively construct more complex elements
    for (int iteration = 0; iteration < complexity_bound; ++iteration) {
        for (int i = 0; i <= iteration; ++i) {
            // 5.1. AndConceptElement:
            for (const auto& c1 : concept_elements_by_complexity[i]) {
                for (const auto& c2 : concept_elements_by_complexity[iteration]) {
                    if (c1.repr() == c2.repr()) continue;
                    dlp::core::ConceptElement concept_element = factory.make_and_concept_element(c1, c2);
                    bool unique = concept_element_cache.insert(concept_element.repr()).second;
                    if (unique && concept_element.complexity() < complexity_bound) {
                        concept_elements_by_complexity[concept_element.complexity()].emplace_back(concept_element);
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
