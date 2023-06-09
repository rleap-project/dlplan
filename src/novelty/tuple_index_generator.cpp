#include "tuple_index_generator.h"


namespace dlplan::novelty {

template<>
void for_each_tuple_index<1>(
    const NoveltyBase &novelty_base,
    AtomIndices atom_indices,
    AtomIndices add_atom_indices,
    const std::function<bool(int)>& callback) {
    assert(novelty_base.get_arity() == 1);
    assert(std::is_sorted(atom_indices.begin(), atom_indices.end()));
    assert(std::is_sorted(add_atom_indices.begin(), add_atom_indices.end()));
    int place_holder = novelty_base.get_num_atoms();
    // add placeholders to be able to generate tuples of size less than 1, i.e., the empty tuple.
    add_atom_indices.push_back(place_holder);
    AtomIndices atom_tuple_indices(1);
    for (int atom_index : add_atom_indices) {
        atom_tuple_indices[0] = atom_index;
        int tuple_index = novelty_base.atom_tuple_to_tuple_index(atom_tuple_indices);
        if (callback(tuple_index)) {
            break;
        }
    }
}

/*
template<>
void for_each_tuple_index<2>(
    const NoveltyBase &novelty_base,
    AtomIndices atom_indices,
    AtomIndices add_atom_indices,
    const std::function<bool(int)>& callback) {
    assert(novelty_base.get_arity() == 2);
    assert(std::is_sorted(atom_indices.begin(), atom_indices.end()));
    assert(std::is_sorted(add_atom_indices.begin(), add_atom_indices.end()));
    int place_holder = novelty_base.get_num_atoms();
    // add placeholders to be able to generate tuples of size less than 2.
    atom_indices.push_back(place_holder);
    add_atom_indices.push_back(place_holder);
    std::vector<int> ref;
    for (int n = 1; n <= 2; ++n) {
        int m = 2 - n;
        // take n values from add_atom_indices and j values from atom_indices
        ref.clear();
        ref.insert(ref.begin(), n, 0);
        ref.insert(ref.begin() + n, m, 1);
        do {
            std::vector<int> indices(2);
            for (int i = 1; i <= n; ++i) {
                indices.push_back(add_atom_indices[])
            }
        } while (std::next_permutation(ref.begin(), ref.end()));
    }
}
*/

}