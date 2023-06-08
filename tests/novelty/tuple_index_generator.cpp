#include <gtest/gtest.h>

#include "../../include/dlplan/novelty.h"

#include "../../src/novelty/tuple_index_generator.h"

#include <iostream>

using namespace dlplan::novelty;


namespace dlplan::tests::novelty {

TEST(DLPTests, TupleIndexGeneratorArity0Test) {
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 0);
    auto tuple_index_generator = TupleIndexGenerator(novelty_base, {}, {0,1});
    auto begin = tuple_index_generator.begin();
    auto end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 0);  // {}
    EXPECT_EQ(begin, end);
}


TEST(DLPTests, TupleIndexGeneratorArity1Test) {
    // Test for more than one atom tuple.
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 1);
    auto tuple_index_generator = TupleIndexGenerator<1>(novelty_base, {}, {0,1,2});
    auto begin = tuple_index_generator.begin();
    auto end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 0);  // {0} 0*5^0
    EXPECT_EQ(*(begin++), 1);  // {1} 1*5^0
    EXPECT_EQ(*(begin++), 2);  // {2} 1*5^0
    EXPECT_EQ(*(begin++), 4);  // {} 0*5^0
    EXPECT_EQ(begin, end);
}


TEST(DLPTests, TupleIndexGeneratorArity2Test) {
    // Test with single atom tuple.
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 2);
    auto tuple_index_generator = TupleIndexGenerator(novelty_base, {}, {0,1});
    auto begin = tuple_index_generator.begin();
    auto end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 5);  // {0,1} 0*5^0 + 1*5^1
    EXPECT_EQ(*(begin++), 20);  // {0,4} 0*5^0 + 4*5^1
    EXPECT_EQ(*(begin++), 21);  // {1,4} 1*5^0 + 4*5^1
    EXPECT_EQ(*(begin++), 24);  // {4,4} 4*5^0 + 4*5^1
    EXPECT_EQ(begin, end);

    // Test with unsorted atom indices, we get the same result
    tuple_index_generator = TupleIndexGenerator(novelty_base, {}, {1,0});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 5);  // {0,1} 5 = 0*5^0 + 1*5^1
    EXPECT_EQ(*(begin++), 20);  // {0,4} 5 = 0*5^0 + 4*5^1
    EXPECT_EQ(*(begin++), 21);  // {1,4} 5 = 1*5^0 + 4*5^1
    EXPECT_EQ(*(begin++), 24);  // {4,4} 5 = 4*5^0 + 4*5^1
    EXPECT_EQ(begin, end);

    // Test with empty atom indices.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {}, {});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 24);  // {4,4} 4*5^0 + 4*5^1
    EXPECT_EQ(begin, end);

    // Test for more than one atom tuple.
    tuple_index_generator = TupleIndexGenerator(novelty_base, {}, {0,1,2});
    begin = tuple_index_generator.begin();
    end = tuple_index_generator.end();
    EXPECT_EQ(*(begin++), 5);  // {0,1} 0*5^0 + 1*5^1
    EXPECT_EQ(*(begin++), 10);  // {0,2} 0*5^0 + 2*5^1
    EXPECT_EQ(*(begin++), 20);  // {0,4} 0*5^0 + 4*5^1
    EXPECT_EQ(*(begin++), 11);  // {1,2} 1*5^0 + 2*5^1
    EXPECT_EQ(*(begin++), 21);  // {1,4} 1*5^0 + 4*5^1
    EXPECT_EQ(*(begin++), 22);  // {2,4} 2*5^0 + 4*5^1
    EXPECT_EQ(*(begin++), 24);  // {4,4} 4*5^0 + 4*5^1
    EXPECT_EQ(begin, end);
}

}
