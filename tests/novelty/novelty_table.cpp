#include <gtest/gtest.h>

#include "../../include/dlplan/novelty.h"

#include <iostream>

using namespace dlplan::novelty;


namespace dlplan::tests::novelty {

TEST(DLPTests, NoveltyBaseTableInsertAtomIndices1Test) {
    auto novelty_base = std::make_shared<const NoveltyBase>(5, 2);
    auto novelty_table = NoveltyTable(novelty_base);
    bool is_novel = novelty_table.insert_atom_indices({0,1,2});
    EXPECT_EQ(is_novel, true);
    is_novel = novelty_table.insert_atom_indices({2,3,4});
    EXPECT_EQ(is_novel, true);
    is_novel = novelty_table.insert_atom_indices({1,2,3});
    EXPECT_EQ(is_novel, true);
    is_novel = novelty_table.insert_atom_indices({2,3,4});
    EXPECT_EQ(is_novel, false);
}


TEST(DLPTests, NoveltyBaseTableInsertAtomIndices2Test) {
    auto novelty_base = std::make_shared<const NoveltyBase>(5, 2);
    auto novelty_table = NoveltyTable(novelty_base);
    bool is_novel = novelty_table.insert_atom_indices({0}, {1,2});
    EXPECT_EQ(is_novel, true);
    is_novel = novelty_table.insert_atom_indices({2}, {3,4});
    EXPECT_EQ(is_novel, true);
    is_novel = novelty_table.insert_atom_indices({}, {1,2,3});
    EXPECT_EQ(is_novel, true);
    is_novel = novelty_table.insert_atom_indices({}, {2,3,4});
    EXPECT_EQ(is_novel, false);

    auto novelty_base_2 = std::make_shared<const NoveltyBase>(6,2);
    novelty_table.resize(novelty_base_2);
    is_novel = novelty_table.insert_atom_indices({}, {4,5}, false);
    EXPECT_EQ(is_novel, true);
    is_novel = novelty_table.insert_atom_indices({}, {4,5}, false);
    EXPECT_EQ(is_novel, false);
}


TEST(DLPTests, NoveltyBaseTableComputeNovelTupleIndices1Test) {
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 2);
    auto novelty_table = NoveltyTable(novelty_base);
    auto tuple_indices = novelty_table.compute_novel_tuple_indices({0,1,2});
    std::sort(tuple_indices.begin(), tuple_indices.end());
    EXPECT_EQ(tuple_indices, TupleIndices({0, 5, 10, 11, 15, 16, 17}));
}


TEST(DLPTests, NoveltyBaseTableComputeNovelTupleIndices2Test) {
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 2);
    auto novelty_table = NoveltyTable(novelty_base);
    auto tuple_indices_1 = novelty_table.compute_novel_tuple_indices({}, {0,1,2});
    std::sort(tuple_indices_1.begin(), tuple_indices_1.end());
    EXPECT_EQ(tuple_indices_1, TupleIndices({5, 10, 11, 15, 16, 17}));

    auto tuple_indices_2 = novelty_table.compute_novel_tuple_indices({1,3}, {0,2});
    std::sort(tuple_indices_2.begin(), tuple_indices_2.end());
    EXPECT_EQ(tuple_indices_2, TupleIndices({5, 11, 15, 16, 17, 21, 23}));
}

TEST(DLPTests, NoveltyBaseTableInsertTupleIndicesTest) {
    auto novelty_base = std::make_shared<const NoveltyBase>(4, 2);
    auto novelty_table = NoveltyTable(novelty_base);
    auto is_novel = novelty_table.insert_tuple_indices({0,1,2}, false);
    EXPECT_EQ(is_novel, true);
    is_novel = novelty_table.insert_tuple_indices({3,4,5}, false);
    EXPECT_EQ(is_novel, true);
    is_novel = novelty_table.insert_tuple_indices({1,2,3,4}, false);
    EXPECT_EQ(is_novel, false);

    is_novel = novelty_table.insert_tuple_indices({10,11,12}, true);
    EXPECT_EQ(is_novel, true);
    is_novel = novelty_table.insert_tuple_indices({13,14,15}, true);
    EXPECT_EQ(is_novel, true);
    is_novel = novelty_table.insert_tuple_indices({11,12,13,14}, true);
    EXPECT_EQ(is_novel, true);
}

}
