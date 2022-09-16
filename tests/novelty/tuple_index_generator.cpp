#include <gtest/gtest.h>

#include <iostream>

#include "../include/dlplan/novelty.h"


using namespace dlplan::novelty;

TEST(DLPTests, TupleIndexGeneratorTest) {
    // Test with single atom tuple.
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 2);
    auto tuple_index_generator = TupleIndexGenerator(novelty_base, {0,1});
    auto begin = tuple_index_generator.begin();
    auto end = tuple_index_generator.end();
    ASSERT_EQ(*(begin++), 4);  // {0,1} 4 = 0*4^0 + 1*4^1
    ASSERT_EQ(begin, end);

    // Test for more than one atom tuple.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {0,1,2,3});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    ASSERT_EQ(*(begin++), 8);  // {0,2} 8 = 0*4^0 + 2*4^1
    ASSERT_EQ(*(begin++), 12);  // {0,3} 12 = 0*4^0 + 3*4^1
    ASSERT_EQ(*(begin++), 9);  // {1,2} 9 = 1*4^0 + 2*4^1
    ASSERT_EQ(*(begin++), 13);  // {1,3} 13 = 1*4^0 + 3*4^1
    ASSERT_EQ(*(begin++), 14);  // {2,3} 14 = 2*4^0 + 3*4^1
    ASSERT_EQ(*(begin++), 4);  // {0,1} 14 = 0*4^0 + 1*4^1  - {0,1} occurs in the end because we call seek_next() in the constructor
    ASSERT_EQ(begin, end);
}
