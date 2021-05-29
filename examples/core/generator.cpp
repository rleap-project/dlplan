#include "../../include/dlp/core.h"

#include <iostream>
#include <unordered_set>

static bool try_insert_uniquely(const std::string& repr, std::unordered_set<std::string>& cache) {
    if (cache.find(repr) == cache.end()) {
        cache.insert(repr);
        return true;
    }
    return false;
}


/**
 * An example that illustrates using the core component in the generator component.
 */
int main() {
    // 1. Initialize planning instance
    dlp::core::InstanceInfo instance;
    // Add state atoms
    dlp::core::Atom a0 = instance.add_atom("on_A_B", "on", {"A", "B"}, false);
    dlp::core::Atom a1 = instance.add_atom("on_B_A", "on", {"B", "A"}, false);
    dlp::core::Atom a2 = instance.add_atom("onTable_A", "onTable", {"A"}, false);
    dlp::core::Atom a3 = instance.add_atom("onTable_B", "onTable", {"B"}, false);
    dlp::core::Atom a4 = instance.add_atom("holding_A", "holding", {"A"}, false);
    dlp::core::Atom a5 = instance.add_atom("holding_B", "holding", {"B"}, false);
    // Add goal atoms
    dlp::core::Atom a6 = instance.add_atom("on_g_A_B", "on_g", {"A", "B"}, true);

    // 2. Initialize factory.
    dlp::core::ElementFactory factory;

    // Equivalence checking using repr.
    std::unordered_set<std::string> concept_element_cache;
    std::unordered_set<std::string> role_element_cache;
    std::unordered_set<std::string> numerical_element_cache;
    std::unordered_set<std::string> boolean_element_cache;
    // Constructed elements sorted by complexity for incremental construction.
    int complexity_bound = 5;
    std::vector<std::vector<dlp::core::ConceptElement>> concept_elements_by_complexity(complexity_bound);
    std::vector<std::vector<dlp::core::RoleElement>> role_elements_by_complexity(complexity_bound);
    std::vector<std::vector<dlp::core::NumericalElement>> numerical_elements_by_complexity(complexity_bound);
    std::vector<std::vector<dlp::core::BooleanElement>> boolean_elements_by_complexity(complexity_bound);

    // construct base
    std::vector<dlp::core::Predicate> predicates = instance.predicates();
    for (const dlp::core::Predicate& predicate: predicates) {
        for (unsigned pos = 0; pos < predicate.arity(); ++pos) {
            dlp::core::ConceptElement concept_element = factory.make_primitive_concept_element(instance, predicate.name(), pos);
            bool added = try_insert_uniquely(concept_element.repr(), concept_element_cache);
            if (added) concept_elements_by_complexity[0].emplace_back(concept_element);
        }
        for (unsigned pos1 = 0; pos1 < predicate.arity(); ++pos1) {
            for (unsigned pos2 = 0; pos2 < predicate.arity(); ++pos2) {
                // add roles
            }
        }
    }
    // inductively build more complex elements
    for (int iteration = 1; iteration < complexity_bound; ++iteration) {

    }

    std::cout << "Total concept elements: " << concept_element_cache.size() << std::endl;
    std::cout << "Total role elements: " << role_element_cache.size() << std::endl;
    std::cout << "Total numerical elements: " << numerical_element_cache.size() << std::endl;
    std::cout << "Total boolean elements: " << boolean_element_cache.size() << std::endl;
};
