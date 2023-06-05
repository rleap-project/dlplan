#include <gtest/gtest.h>

#include "../../include/dlplan/novelty.h"

#include "../../src/novelty/tuple_index_generator.h"

#include <iostream>

using namespace dlplan::novelty;


namespace dlplan::tests::novelty {

TEST(DLPTests, TupleIndexGeneratorTest) {
    // Test with single atom tuple.
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 2);
    auto tuple_index_generator = TupleIndexGenerator(novelty_base, {0,1}, {});
    auto begin = tuple_index_generator.begin();
    auto end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 11);  // {0,1} 11 = 1*5^0 + 2*5^1
    EXPECT_EQ(begin, end);

    // Test with unsorted atom indices.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {1,0}, {});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 11);  // {0,1} 11 = 1*5^0 + 2*5^1
    EXPECT_EQ(begin, end);

    // Test with empty atom indices.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {}, {});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 0);  // {} 0
    EXPECT_EQ(begin, end);

    // Test for more than one atom tuple.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {0,1,2,3}, {});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 11);  // {0,1} 11 = 1*5^0 + 2*5^1
    EXPECT_EQ(*(begin++), 16);  // {0,2} 16 = 1*5^0 + 3*5^1
    EXPECT_EQ(*(begin++), 21);  // {0,3} 21 = 1*5^0 + 4*5^1
    EXPECT_EQ(*(begin++), 17);  // {1,2} 17 = 2*5^0 + 3*5^1
    EXPECT_EQ(*(begin++), 22);  // {1,3} 22 = 2*5^0 + 4*5^1
    EXPECT_EQ(*(begin++), 23);  // {2,3} 23 = 3*5^0 + 4*5^1
    EXPECT_EQ(begin, end);
}

TEST(DLPTests, TupleIndexGeneratorTest2) {
    // Test with single atom tuple.
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 3);
    auto tuple_index_generator = TupleIndexGenerator(novelty_base, {0,1,2}, {});
    auto begin = tuple_index_generator.begin();
    auto end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 86);  // {0,1,2} 86 = 1*5^0 + 2*5^1 + 3*5^2
    EXPECT_EQ(begin, end);

    // Test with unsorted atom indices.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {2,1,0}, {});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 86);  // {0,1,2} 86 = 1*5^0 + 2*5^1 + 3*5^2
    EXPECT_EQ(begin, end);

    // Test with empty atom indices.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {0}, {});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 1);  // {0} 1 = 1*5^0
    EXPECT_EQ(begin, end);

    // Test for more than one atom tuple.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {0,1,2,3}, {});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 86);  // {0,1,2} 86 = 1*5^0 + 2*5^1 + 3*5^2
    EXPECT_EQ(*(begin++), 111);  // {0,1,3} 111 = 1*5^0 + 2*5^1 + 4*5^2
    EXPECT_EQ(*(begin++), 116);  // {0,2,3} 116 = 1*5^0 + 3*5^1 + 4*5^2
    EXPECT_EQ(*(begin++), 117);  // {1,2,3} 117 = 2*5^0 + 3*5^1 + 4*5^2
    EXPECT_EQ(begin, end);
}

TEST(DLPTests, TupleIndexGeneratorArity1) {
    // Test with single atom tuple.
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 1);
    auto tuple_index_generator = TupleIndexGenerator<1>(novelty_base, {0,1,2}, {});
    auto begin = tuple_index_generator.begin();
    auto end = tuple_index_generator.end();
}

}
