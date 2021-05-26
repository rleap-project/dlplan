#include "include/dlp/core.h"


int main() {
    // Initialize planning instance
    dlp::core::InstanceInfo instance;
    instance.add_atom("on", {"A", "B"}, false, true);
    instance.add_atom("on", {"B", "A"}, false, false);

    // Initialize factory.
    dlp::core::ElementFactory factory;

    // Construct and evaluate elements.
    // Parse text to NumericalElement
    dlp::core::NumericalElement numerical = factory.parse_numerical_element(instance, "n_count(c_and(on_g(0),on(0)))");
    // Parse text to ConceptElement: returns the ConceptElement that is already cached
    dlp::core::ConceptElement concept = factory.parse_concept_element(instance, "c_and(on(0),on_g(0))");
    // Evaluate NumericalElement on state.
    dlp::core::State state = instance.convert_state({1,2,42});
    int value = numerical.evaluate(state);
};
