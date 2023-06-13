#include <gtest/gtest.h>

#include "../../include/dlplan/novelty.h"

#include "../../src/novelty/tuple_index_generator.h"

#include <iostream>

using namespace dlplan::novelty;


namespace dlplan::tests::novelty {

TEST(DLPLTests, TupleIndexGeneratorForEachArity1Test) {
    auto novelty_base = NoveltyBase(4, 1);
    // Generate all tuples up to size 1.
    TupleIndices result_1;
    for_each_tuple_index<1>(novelty_base, {0,1,2}, [&](TupleIndex tuple_index){
        result_1.push_back(tuple_index);
        return false;
    });
    std::sort(result_1.begin(), result_1.end());
    EXPECT_EQ(result_1, TupleIndices({0,1,2,4}));
    std::vector<AtomIndices> atom_tuple_indices_1;
    std::for_each(result_1.begin(), result_1.end(), [&](TupleIndex tuple_index){
        atom_tuple_indices_1.push_back(novelty_base.tuple_index_to_atom_indices(tuple_index));
    });
    EXPECT_EQ(atom_tuple_indices_1, std::vector<AtomIndices>({{0}, {1}, {2}, {}}));

    // Generate all tuples up to size 1 that include at least one atom from add_atom_indices.
    TupleIndices result_2;
    for_each_tuple_index<1>(novelty_base, {}, {0,1,2}, [&](TupleIndex tuple_index){
        result_2.push_back(tuple_index);
        return false;
    });
    std::sort(result_2.begin(), result_2.end());
    EXPECT_EQ(result_2, TupleIndices({0,1,2}));
    std::vector<AtomIndices> atom_tuple_indices_2;
    std::for_each(result_2.begin(), result_2.end(), [&](TupleIndex tuple_index){
        atom_tuple_indices_2.push_back(novelty_base.tuple_index_to_atom_indices(tuple_index));
    });
    EXPECT_EQ(atom_tuple_indices_2, std::vector<AtomIndices>({{0}, {1}, {2}}));

    TupleIndices result_3;
    for_each_tuple_index<1>(novelty_base, {1,3}, {0,2}, [&](TupleIndex tuple_index){
        result_3.push_back(tuple_index);
        return false;
    });
    std::sort(result_3.begin(), result_3.end());
    EXPECT_EQ(result_3, TupleIndices({0,2}));
    std::vector<AtomIndices> atom_tuple_indices_3;
    std::for_each(result_3.begin(), result_3.end(), [&](TupleIndex tuple_index){
        atom_tuple_indices_3.push_back(novelty_base.tuple_index_to_atom_indices(tuple_index));
    });
    EXPECT_EQ(atom_tuple_indices_3, std::vector<AtomIndices>({{0}, {2}}));

    // Test empty vectors.
    TupleIndices result_4;
    for_each_tuple_index<1>(novelty_base, {}, [&](TupleIndex tuple_index){
        result_4.push_back(tuple_index);
        return false;
    });
    std::sort(result_4.begin(), result_4.end());
    EXPECT_EQ(result_4, TupleIndices({4}));
    std::vector<AtomIndices> atom_tuple_indices_4;
    std::for_each(result_4.begin(), result_4.end(), [&](TupleIndex tuple_index){
        atom_tuple_indices_4.push_back(novelty_base.tuple_index_to_atom_indices(tuple_index));
    });
    EXPECT_EQ(atom_tuple_indices_4, std::vector<AtomIndices>({{}}));

    TupleIndices result_5;
    for_each_tuple_index<1>(novelty_base, {}, {}, [&](TupleIndex tuple_index){
        result_5.push_back(tuple_index);
        return false;
    });
    std::sort(result_5.begin(), result_5.end());
    EXPECT_EQ(result_5, TupleIndices());
    std::vector<AtomIndices> atom_tuple_indices_5;
    std::for_each(result_5.begin(), result_5.end(), [&](TupleIndex tuple_index){
        atom_tuple_indices_5.push_back(novelty_base.tuple_index_to_atom_indices(tuple_index));
    });
    EXPECT_EQ(atom_tuple_indices_5, std::vector<AtomIndices>());
}


TEST(DLPLTests, TupleIndexGeneratorForEachArity2Test) {
    auto novelty_base = NoveltyBase(4, 2);
    // Generate all tuples up to size 2.
    TupleIndices result_1;
    for_each_tuple_index<2>(novelty_base, {0,1,2}, [&](TupleIndex tuple_index){
        result_1.push_back(tuple_index);
        return false;
    });
    std::sort(result_1.begin(), result_1.end());
    EXPECT_EQ(result_1, TupleIndices({5,10,11,20,21,22,24}));
    std::vector<AtomIndices> atom_tuple_indices_1;
    std::for_each(result_1.begin(), result_1.end(), [&](TupleIndex tuple_index){
        atom_tuple_indices_1.push_back(novelty_base.tuple_index_to_atom_indices(tuple_index));
    });
    EXPECT_EQ(atom_tuple_indices_1, std::vector<AtomIndices>({{0,1}, {0,2}, {1,2}, {0}, {1}, {2}, {}}));

    // Generate all tuples up to size 2 that include at least one atom from add_atom_indices.
    TupleIndices result_2;
    for_each_tuple_index<2>(novelty_base, {}, {0,1,2}, [&](TupleIndex tuple_index){
        result_2.push_back(tuple_index);
        return false;
    });
    std::sort(result_2.begin(), result_2.end());
    EXPECT_EQ(result_2, TupleIndices({5,10,11,20,21,22}));
    std::vector<AtomIndices> atom_tuple_indices_2;
    std::for_each(result_2.begin(), result_2.end(), [&](TupleIndex tuple_index){
        atom_tuple_indices_2.push_back(novelty_base.tuple_index_to_atom_indices(tuple_index));
    });
    EXPECT_EQ(atom_tuple_indices_2, std::vector<AtomIndices>({{0,1}, {0,2}, {1,2}, {0}, {1}, {2}}));

    TupleIndices result_3;
    for_each_tuple_index<2>(novelty_base, {1,3}, {0,2}, [&](TupleIndex tuple_index){
        result_3.push_back(tuple_index);
        return false;
    });
    std::sort(result_3.begin(), result_3.end());
    EXPECT_EQ(result_3, TupleIndices({5,10,11,15,17,20,22}));
    std::vector<AtomIndices> atom_tuple_indices_3;
    std::for_each(result_3.begin(), result_3.end(), [&](TupleIndex tuple_index){
        atom_tuple_indices_3.push_back(novelty_base.tuple_index_to_atom_indices(tuple_index));
    });
    EXPECT_EQ(atom_tuple_indices_3, std::vector<AtomIndices>({{0,1}, {0,2}, {1,2}, {0,3}, {2,3}, {0}, {2}}));

    // Test empty vectors
    TupleIndices result_4;
    for_each_tuple_index<2>(novelty_base, {}, [&](TupleIndex tuple_index){
        result_4.push_back(tuple_index);
        return false;
    });
    std::sort(result_4.begin(), result_4.end());
    EXPECT_EQ(result_4, TupleIndices({24}));
    std::vector<AtomIndices> atom_tuple_indices_4;
    std::for_each(result_4.begin(), result_4.end(), [&](TupleIndex tuple_index){
        atom_tuple_indices_4.push_back(novelty_base.tuple_index_to_atom_indices(tuple_index));
    });
    EXPECT_EQ(atom_tuple_indices_4, std::vector<AtomIndices>({{}}));

    TupleIndices result_5;
    for_each_tuple_index<2>(novelty_base, {}, {}, [&](TupleIndex tuple_index){
        result_5.push_back(tuple_index);
        return false;
    });
    std::sort(result_5.begin(), result_5.end());
    EXPECT_EQ(result_5, TupleIndices());
    std::vector<AtomIndices> atom_tuple_indices_5;
    std::for_each(result_5.begin(), result_5.end(), [&](TupleIndex tuple_index){
        atom_tuple_indices_5.push_back(novelty_base.tuple_index_to_atom_indices(tuple_index));
    });
    EXPECT_EQ(atom_tuple_indices_5, std::vector<AtomIndices>());
}

TEST(DLPLTests, TupleIndexGeneratorForEachArityKTest) {
    auto novelty_base = NoveltyBase(4, 2);
    // Generate all tuples up to size 2.
    TupleIndices result_1;
    for_each_tuple_index(novelty_base, {0,1,2}, [&](TupleIndex tuple_index){
        result_1.push_back(tuple_index);
        return false;
    });
    std::sort(result_1.begin(), result_1.end());
    EXPECT_EQ(result_1, TupleIndices({5,10,11,20,21,22,24}));
    std::vector<AtomIndices> atom_tuple_indices_1;
    std::for_each(result_1.begin(), result_1.end(), [&](TupleIndex tuple_index){
        atom_tuple_indices_1.push_back(novelty_base.tuple_index_to_atom_indices(tuple_index));
    });
    EXPECT_EQ(atom_tuple_indices_1, std::vector<AtomIndices>({{0,1}, {0,2}, {1,2}, {0}, {1}, {2}, {}}));

    // Generate all tuples up to size 2 that include at least one atom from add_atom_indices.
    TupleIndices result_2;
    for_each_tuple_index(novelty_base, {}, {0,1,2}, [&](TupleIndex tuple_index){
        result_2.push_back(tuple_index);
        return false;
    });
    std::sort(result_2.begin(), result_2.end());
    EXPECT_EQ(result_2, TupleIndices({5,10,11,20,21,22}));
    std::vector<AtomIndices> atom_tuple_indices_2;
    std::for_each(result_2.begin(), result_2.end(), [&](TupleIndex tuple_index){
        atom_tuple_indices_2.push_back(novelty_base.tuple_index_to_atom_indices(tuple_index));
    });
    EXPECT_EQ(atom_tuple_indices_2, std::vector<AtomIndices>({{0,1}, {0,2}, {1,2}, {0}, {1}, {2}}));

    TupleIndices result_3;
    for_each_tuple_index(novelty_base, {1,3}, {0,2}, [&](TupleIndex tuple_index){
        result_3.push_back(tuple_index);
        return false;
    });
    std::sort(result_3.begin(), result_3.end());
    EXPECT_EQ(result_3, TupleIndices({5,10,11,15,17,20,22}));
    std::vector<AtomIndices> atom_tuple_indices_3;
    std::for_each(result_3.begin(), result_3.end(), [&](TupleIndex tuple_index){
        atom_tuple_indices_3.push_back(novelty_base.tuple_index_to_atom_indices(tuple_index));
    });
    EXPECT_EQ(atom_tuple_indices_3, std::vector<AtomIndices>({{0,1}, {0,2}, {1,2}, {0,3}, {2,3}, {0}, {2}}));

    // Test empty vectors
    TupleIndices result_4;
    for_each_tuple_index(novelty_base, {}, [&](TupleIndex tuple_index){
        result_4.push_back(tuple_index);
        return false;
    });
    std::sort(result_4.begin(), result_4.end());
    EXPECT_EQ(result_4, TupleIndices({24}));
    std::vector<AtomIndices> atom_tuple_indices_4;
    std::for_each(result_4.begin(), result_4.end(), [&](TupleIndex tuple_index){
        atom_tuple_indices_4.push_back(novelty_base.tuple_index_to_atom_indices(tuple_index));
    });
    EXPECT_EQ(atom_tuple_indices_4, std::vector<AtomIndices>({{}}));

    TupleIndices result_5;
    for_each_tuple_index(novelty_base, {}, {}, [&](TupleIndex tuple_index){
        result_5.push_back(tuple_index);
        return false;
    });
    std::sort(result_5.begin(), result_5.end());
    EXPECT_EQ(result_5, TupleIndices());
    std::vector<AtomIndices> atom_tuple_indices_5;
    std::for_each(result_5.begin(), result_5.end(), [&](TupleIndex tuple_index){
        atom_tuple_indices_5.push_back(novelty_base.tuple_index_to_atom_indices(tuple_index));
    });
    EXPECT_EQ(atom_tuple_indices_5, std::vector<AtomIndices>());
}

}
