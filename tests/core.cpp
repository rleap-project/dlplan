
#include <gtest/gtest.h>
#include "dlp/core.h"

// Demonstrate some basic assertions.
TEST(DLPTests, InstanceCreation) {
    dlp::core::InstanceInfo instance;
    dlp::core::Atom a0 = instance.add_atom("on", {"A", "B"}, false);
    dlp::core::Atom a1 = instance.add_atom("on", {"B", "A"}, false);
    dlp::core::Atom a2 = instance.add_atom("onTable", {"A"}, false);
    dlp::core::Atom a3 = instance.add_atom("onTable", {"B"}, false);
    dlp::core::Atom a4 = instance.add_atom("holding", {"A"}, false);
    dlp::core::Atom a5 = instance.add_atom("holding", {"B"}, false);
    dlp::core::Atom a6 = instance.add_atom("on_g", {"A", "B"}, true);

    dlp::core::SyntacticElementFactory factory;

    dlp::core::NumericalElement numerical = factory.parse_numerical_element("n_count(c_and(on_g(0),on(0)))");
    EXPECT_EQ(numerical.complexity(), 4);

    dlp::core::State state = instance.convert_state({a0, a1});
    int value = numerical.evaluate(state);
}