#include "../../include/dlp/core.h"

#include <iostream>


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

    // 3. Construct and evaluate elements.
    // Parse text to NumericalElement
    dlp::core::NumericalElement numerical = factory.parse_numerical_element(instance, "n_count(c_and(on_g(0),on(0)))");
    std::cout << "complexity: " << numerical.complexity() << std::endl;
    // Parse text to ConceptElement: returns the ConceptElement that is already cached
    dlp::core::ConceptElement concept = factory.parse_concept_element(instance, "c_and(on(0),on_g(0))");

    // 4. Evaluate for example state: block A on B
    dlp::core::State state = instance.convert_state({0, 3});
    int value = numerical.evaluate(state);
    std::cout << "value: " << value << std::endl;
    dlp::core::State state2 = instance.parse_state({"on_A_B", "onTable_B"});
    value = numerical.evaluate(state2);
    std::cout << "value: " << value << std::endl;
};
