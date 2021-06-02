
#include <gtest/gtest.h>

#include <dlp/core.h>

using namespace dlp::core;


TEST(DLPTests, SyntacticErrorHandling) {
    VocabularyInfo vocabulary;
    auto clear = vocabulary.add_predicate("on", 2);

    SyntacticElementFactory factory(vocabulary);

    ASSERT_THROW(factory.parse_numerical("hello world!"), std::runtime_error);
    ASSERT_THROW(factory.parse_concept("bye world!"), std::runtime_error);

    ASSERT_THROW(factory.parse_concept("c_sand(on(0),on_g(0))"), std::runtime_error);
    ASSERT_THROW(factory.parse_concept("or(on(0),on_g(0))"), std::runtime_error);
    ASSERT_THROW(factory.parse_concept("c_and(on(0), on(0), on(0))"), std::runtime_error);

    ASSERT_THROW(factory.parse_numerical("n_count(on)"), std::runtime_error);
    ASSERT_THROW(factory.parse_numerical("n_count(on(4))"), std::runtime_error);
    ASSERT_THROW(factory.parse_numerical("n_count(on(-1))"), std::runtime_error);
    ASSERT_THROW(factory.parse_numerical("n_count(on_g(0))"), std::runtime_error);
}


// Demonstrate some basic assertions.
TEST(DLPTests, InstanceCreation) {
    // Add predicates
    VocabularyInfo vocabulary;
    Predicate p0 = vocabulary.add_predicate("on", 2);
    Predicate p1 = vocabulary.add_predicate("onTable", 1);
    Predicate p2 = vocabulary.add_predicate("holding", 1);
    Predicate p3 = vocabulary.add_predicate("on_g", 2);
    std::vector<Predicate> predicates({p0, p1, p2, p3});
    InstanceInfo instance(vocabulary);
    // Add state atoms
    Atom a0 = instance.add_atom("on", {"A", "B"});
    Atom a1 = instance.add_atom("on", {"B", "A"});
    Atom a2 = instance.add_atom("onTable", {"A"});
    Atom a3 = instance.add_atom("onTable", {"B"});
    Atom a4 = instance.add_atom("holding", {"A"});
    Atom a5 = instance.add_atom("holding", {"B"});
    // Add goal atoms
    Atom a6 = instance.add_static_atom("on_g", {"A", "B"});

    SyntacticElementFactory factory(vocabulary);

    Numerical numerical = factory.parse_numerical("n_count(c_and(on_g(0),on(0)))");
    EXPECT_EQ(numerical.compute_complexity(), 4);

    State state = instance.convert_state({a0, a1});
    EXPECT_EQ(numerical.evaluate(state), 1);
}
