#include "include/dlp/core.h"


int main() {
    dlp::core::ElementFactory factory;
    // Add planning instance atoms.
    factory.add_atom("on", {"A", "B"}, true);
    factory.add_atom("on", {"B", "A"}, false);

    // Parse text to NumericalElement
    dlp::core::NumericalElement numerical = factory.make_numerical_element("n_count(c_and(g_on(0),s_on(0)))");
    // Parse text to ConceptElement: returns the ConceptElement that is already cached
    dlp::core::ConceptElement concept = factory.make_concept_element("c_and(s_on(0),g_on(0))");
    // Evaluate NumericalElement on state.
    dlp::core::Index_Vec state({1,2,42});
    int value = numerical.evaluate(state);
};
