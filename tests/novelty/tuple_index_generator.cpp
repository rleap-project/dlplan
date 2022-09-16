#include <gtest/gtest.h>

#include <iostream>

#include "../include/dlplan/novelty.h"


using namespace dlplan::novelty;

TEST(DLPTests, TupleIndexGeneratorTest) {
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 2);
    auto tuple_index_generator = TupleIndexGenerator(novelty_base, {0,1});
    ASSERT_EQ(*tuple_index_generator.begin(), 4);
}
