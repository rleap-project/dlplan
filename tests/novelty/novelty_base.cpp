#include <gtest/gtest.h>

#include "../../include/dlplan/novelty.h"

#include <iostream>

using namespace dlplan::novelty;


namespace dlplan::tests::novelty {

TEST(DLPTests, NoveltyBaseWidthZeroTest) {
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 0);
    EXPECT_EQ(novelty_base->atom_indices_to_tuple_index({}), 0);
}


TEST(DLPTests, NoveltyBaseWidthKTest) {
    // Perfect hashing even duplicates
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 3);
    int p = novelty_base->get_num_atoms();  // place_holder
    EXPECT_EQ(novelty_base->atom_indices_to_tuple_index({p,p,p}), 124);  // 4*5^0 + 4*5^1 + 4*5^2
    EXPECT_EQ(novelty_base->atom_indices_to_tuple_index({0,p,p}), 120);  // 0*5^0 + 4*5^1 + 4*5^2
    EXPECT_EQ(novelty_base->atom_indices_to_tuple_index({0,1,p}), 105);  // 0*5^0 + 1*5^1 + 4*5^2
    // Invert operation.
    EXPECT_EQ(novelty_base->tuple_index_to_atom_indices(124), AtomIndices({}));
    EXPECT_EQ(novelty_base->tuple_index_to_atom_indices(120), AtomIndices({0}));
    EXPECT_EQ(novelty_base->tuple_index_to_atom_indices(105), AtomIndices({0,1}));
}

}
