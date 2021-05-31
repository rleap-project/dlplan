
#include <gtest/gtest.h>
#include "dlp/core.h"

// Demonstrate some basic assertions.
TEST(DLPTests, InstanceCreation) {
    dlp::core::InstanceInfo instance;
    dlp::core::Atom a0 = instance.add_atom("on", {"A", "B"}, false);
    dlp::core::Atom a1 = instance.add_atom("on", {"B", "A"}, false);

    dlp::core::SyntacticElementFactory factory;

    dlp::core::NumericalElement numerical = factory.parse_numerical_element("n_count(c_and(on_g(0),on(0)))");
    EXPECT_EQ(numerical.complexity(), 4);

    // TODO This raises some errors?
//    dlp::core::State state = instance.convert_state({0, 3});
//    int value = numerical.evaluate(state);
}