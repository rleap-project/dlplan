#include <gtest/gtest.h>

#include "../../include/dlplan/novelty.h"

#include <iostream>

using namespace dlplan::novelty;


namespace dlplan::tests::novelty {

TEST(DLPTests, NoveltyBaseWidthZeroTest) {
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 0);
    EXPECT_EQ(novelty_base->atom_tuple_to_tuple_index({}), 0);
}


TEST(DLPTests, NoveltyBaseWidthKTest) {
    // Perfect hashing even duplicates
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 3);
    EXPECT_EQ(novelty_base->atom_tuple_to_tuple_index({0,1}), 11);
    EXPECT_EQ(novelty_base->atom_tuple_to_tuple_index({0,0,1}), 56);
    // Invert operation.
    EXPECT_EQ(novelty_base->tuple_index_to_atom_tuple(11), AtomIndices({0,1}));
    EXPECT_EQ(novelty_base->tuple_index_to_atom_tuple(56), AtomIndices({0,0,1}));
}

}
