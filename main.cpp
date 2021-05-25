#include "include/dlp/core.h"


int main() {
    // Add planning instance atoms.
    dlp::core::InstanceInfo info;
    info.add_atom("on", {"A", "B"}, true);
    info.add_atom("on", {"B", "A"}, false);

    dlp::core::ElementFactory factory;
    // Parse text to NumericalElement
    dlp::core::NumericalElement numerical = factory.make_numerical_element(info, "n_count(c_and(g_on(0),s_on(0)))");
    // Parse text to ConceptElement: returns the ConceptElement that is already cached
    dlp::core::ConceptElement concept = factory.make_concept_element(info, "c_and(s_on(0),g_on(0))");
    // Evaluate NumericalElement on state.
    dlp::core::Index_Vec state({1,2,42});
    int value = numerical.evaluate(state);
};
