#include "include/dlp/core.h"


int main() {
    // Initialize planning instance
    dlp::core::InstanceInfo info;
    info.add_atom("on", {"A", "B"});
    info.add_atom("on", {"B", "A"});

    // Initialize factory.
    dlp::core::ElementFactory factory;

    // Construct and evaluate elements.
    // Parse text to NumericalElement
    dlp::core::NumericalElement numerical = factory.parse_numerical_element(info, "n_count(c_and(g_on(0),s_on(0)))");
    // Parse text to ConceptElement: returns the ConceptElement that is already cached
    dlp::core::ConceptElement concept = factory.parse_concept_element(info, "c_and(s_on(0),g_on(0))");
    // Evaluate NumericalElement on state.
    dlp::core::State state(info, {1,2,42});
    int value = numerical.evaluate(state);
};
