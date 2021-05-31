#include "../../include/dlp/core.h"

#include <iostream>


/**
 * The simplest example to illustrate the construction and evaluation of elements.
 */
int main() {
    // 1. Initialize planning instance
    dlp::core::InstanceInfo instance;
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
    dlp::core::SyntacticElementFactory factory;

    // 3. Construct and evaluate elements.
    // Parse text to NumericalElement
    dlp::core::NumericalElement numerical = factory.parse_numerical_element("n_count(c_and(on_g(0),on(0)))");
    std::cout << "complexity: " << numerical.compute_complexity() << std::endl;
    // Parse text to ConceptElement: returns the ConceptElement that is already cached
    dlp::core::ConceptElement concept = factory.parse_concept_element("c_and(on(0),on_g(0))");

    // 4. Evaluate for example state: block A on B
    dlp::core::State state = instance.convert_state({0, 3});
    int value = numerical.evaluate(state);
    std::cout << "value: " << value << std::endl;
    dlp::core::State state2 = instance.convert_state({a0, a3});
    value = numerical.evaluate(state2);
    std::cout << "value: " << value << std::endl;
};
