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
    EXPECT_EQ(*(begin++), 5);  // {0,1} 5 = 0*5^0 + 1*5^1
    EXPECT_EQ(begin, end);

    // Test with unsorted atom indices.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {1,0});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 5);  // {0,1} 5 = 0*5^0 + 1*5^1
    EXPECT_EQ(begin, end);

    // Test with empty atom indices.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {0});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 25);  // {0,5} 25 = 0*5^0 + 5*5^1
    EXPECT_EQ(begin, end);

    // Test for more than one atom tuple.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {0,1,2,3});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 10);  // {0,2} 10 = 0*5^0 + 2*5^1
    EXPECT_EQ(*(begin++), 15);  // {0,3} 15 = 0*5^0 + 3*5^1
    EXPECT_EQ(*(begin++), 11);  // {1,2} 11 = 1*5^0 + 2*5^1
    EXPECT_EQ(*(begin++), 16);  // {1,3} 16 = 1*5^0 + 3*5^1
    EXPECT_EQ(*(begin++), 17);  // {2,3} 17 = 2*5^0 + 3*5^1
    EXPECT_EQ(*(begin++), 5);  // {0,1} 5 = 0*5^0 + 1*5^1  - {0,1} occurs in the end because we call seek_next() in the constructor
    EXPECT_EQ(begin, end);
}

TEST(DLPTests, TupleIndexGeneratorTest2) {
    // Test with single atom tuple.
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 3);
    auto tuple_index_generator = TupleIndexGenerator(novelty_base, {0,1,2});
    auto begin = tuple_index_generator.begin();
    auto end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 55);  // {0,1,2} 55 = 0*5^0 + 1*5^1 + 2*5^1
    EXPECT_EQ(begin, end);

    // Test with unsorted atom indices.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {2,1,0});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 55);  // {0,1,2} 55 = 0*5^0 + 1*5^1 + 2*5^1
    EXPECT_EQ(begin, end);

    // Test with empty atom indices.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {0});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 150);  // {0,5,5} 150 = 0*5^0 + 5*5^1 + 5*5^2
    EXPECT_EQ(begin, end);

    // Test for more than one atom tuple.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {0,1,2,3});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 80);  // {0,1,3} 80 = 0*5^0 + 1*5^1 + 3*5^2
    EXPECT_EQ(*(begin++), 85);  // {0,2,3} 85 = 0*5^0 + 2*5^1 + 3*5^2
    EXPECT_EQ(*(begin++), 86);  // {1,2,3} 11 = 1*5^0 + 2*5^1 + 3*5^2
    EXPECT_EQ(*(begin++), 55);  // {0,1,2} 16 = 1*5^0 + 3*5^1 + 5*5^2 - {0,1,2} occurs in the end because we call seek_next() in the constructor
    EXPECT_EQ(begin, end);
}