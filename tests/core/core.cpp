#include <gtest/gtest.h>

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, SyntacticErrorHandling) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto clear = vocabulary->add_predicate("on", 2);

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
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("on", 2);
    auto predicate_1 = vocabulary->add_predicate("onTable", 1);
    auto predicate_2 = vocabulary->add_predicate("holding", 1);
    auto predicate_3 = vocabulary->add_predicate("on_g", 2);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0 = instance->add_atom("on", {"A", "B"});
    auto atom_1 = instance->add_atom("on", {"B", "A"});
    auto atom_2 = instance->add_atom("onTable", {"A"});
    auto atom_3 = instance->add_atom("onTable", {"B"});
    auto atom_4 = instance->add_atom("holding", {"A"});
    auto atom_5 = instance->add_atom("holding", {"B"});
    auto atom_6 = instance->add_static_atom("on_g", {"A", "B"});

    SyntacticElementFactory factory(vocabulary);

    std::shared_ptr<const Numerical> numerical = factory.parse_numerical("n_count(c_and(c_primitive(on_g,0),c_primitive(on,0)))");
    EXPECT_EQ(numerical->compute_complexity(), 4);

    State state_0(0, instance, {atom_0, atom_3});
    State state_1(1, instance, {0, 3});
    EXPECT_EQ(state_0, state_1);
    EXPECT_EQ(numerical->evaluate(state_0), 1);
    EXPECT_EQ(numerical->evaluate(state_1), 1);

    State state_2(2, instance, {atom_2, atom_3});
    State state_3(3, instance, {2, 3});
    EXPECT_EQ(state_2, state_3);
    EXPECT_EQ(numerical->evaluate(state_2), 0);
    EXPECT_EQ(numerical->evaluate(state_3), 0);
}

}
