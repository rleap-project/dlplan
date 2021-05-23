#include "include/dlp/core.h"

int main() {
    dlp::ElementFactory factory;
    // Add planning instance atoms.
    factory.add_atom("on", {"A", "B"});
    factory.add_atom("on", {"B", "A"});
    // ...
    // B must be on A in the goal
    factory.set_goal_atoms({1,});
    // no atoms that remain constant.
    factory.set_constant_atoms({});
    // Parse text to NumericalElement
    dlp::NumericalElement numerical(factory, "n_count(c_and(g_on(0),s_on(0)))");
    // Parse text to ConceptElement: this throw and error because n_count cannot be parsed into a ConceptElement
    // dlp::NumericalElement numerical(factory, "n_count(c_and(g_on(0),s_on(0)))");
    // Parse text to ConceptElement: will return the ConceptElement that is already cached.
    dlp::NumericalElement concept(factory, "c_and(s_on(0),g_on(0))");
    // Evaluate NumericalElement on state.
    dlp::Index_Vec state({1,2,42});
    int value = numerical.evaluate(state);
};
