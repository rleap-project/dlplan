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
    ASSERT_EQ(*(begin++), 5);  // {0,1} 5 = 0*5^0 + 1*5^1
    ASSERT_EQ(begin, end);

    // Test with unsorted atom indices.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {1,0});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    ASSERT_EQ(*(begin++), 5);  // {0,1} 5 = 0*5^0 + 1*5^1
    ASSERT_EQ(begin, end);

    // Test with empty atom indices.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {0});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    ASSERT_EQ(*(begin++), 25);  // {0,1} 25 = 0*5^0 + 5*5^1
    ASSERT_EQ(begin, end);

    // Test for more than one atom tuple.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {0,1,2,3});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    ASSERT_EQ(*(begin++), 10);  // {0,2} 10 = 0*5^0 + 2*5^1
    ASSERT_EQ(*(begin++), 15);  // {0,3} 15 = 0*5^0 + 3*5^1
    ASSERT_EQ(*(begin++), 11);  // {1,2} 11 = 1*5^0 + 2*5^1
    ASSERT_EQ(*(begin++), 16);  // {1,3} 16 = 1*5^0 + 3*5^1
    ASSERT_EQ(*(begin++), 17);  // {2,3} 17 = 2*5^0 + 3*5^1
    ASSERT_EQ(*(begin++), 5);  // {0,1} 5 = 0*5^0 + 1*5^1  - {0,1} occurs in the end because we call seek_next() in the constructor
    ASSERT_EQ(begin, end);
}
