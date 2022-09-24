#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, BooleanNullary) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("predicate", 0);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    // Add state atoms
    Atom a0 = instance->add_atom("predicate", {});

    State s0(instance, {a0}, 0);
    State s1(instance, std::vector<Atom>(), 1);

    SyntacticElementFactory factory(vocabulary);
    DenotationsCaches caches;

    Boolean boolean = factory.parse_boolean("b_nullary(predicate)");

    EXPECT_EQ(boolean.evaluate(s0), true);
    EXPECT_EQ(boolean.evaluate(s0, caches), true);
    EXPECT_EQ(boolean.evaluate({s0}, caches), true);

    EXPECT_EQ(boolean.evaluate(s1), false);

}
