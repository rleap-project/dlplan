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
    int p = NoveltyBase::place_holder;  // place_holder
    EXPECT_EQ(novelty_base->atom_indices_to_tuple_index({p,p,p}), 0);  // 4*5^0 + 4*5^1 + 4*5^2
    EXPECT_EQ(novelty_base->atom_indices_to_tuple_index({p,p,0}), 25);  // 0*5^0 + 0*5^1 + 1*5^2
    EXPECT_EQ(novelty_base->atom_indices_to_tuple_index({p,0,1}), 55);  // 0*5^0 + 1*5^1 + 2*5^2
    // Invert operation.
    EXPECT_EQ(novelty_base->tuple_index_to_atom_indices(0), AtomIndices({}));
    EXPECT_EQ(novelty_base->tuple_index_to_atom_indices(25), AtomIndices({0}));
    EXPECT_EQ(novelty_base->tuple_index_to_atom_indices(55), AtomIndices({0,1}));
}

}
