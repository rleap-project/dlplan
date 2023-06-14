#include "tuple_index_generator.h"

#include "../utils/logging.h"


namespace dlplan::novelty {

std::array<std::vector<int>, 2> compute_geq_mappings(
    const AtomIndices& vec_1,
    const AtomIndices& vec_2) {
    std::vector<int> geq_1(vec_1.size(), std::numeric_limits<int>::max());
    std::vector<int> geq_2(vec_2.size(), std::numeric_limits<int>::max());
    int j = 0;
    int i = 0;
    while (j < static_cast<int>(geq_1.size()) && i < static_cast<int>(geq_2.size())) {
        if (vec_1[j] < vec_2[i]) {
            geq_1[j] = i;
            ++j;
        } else if (vec_1[j] > vec_2[i]) {
            geq_2[i] = j;
            ++i;
        } else {
            geq_2[i] = j;
            geq_1[j] = i;
            ++j;
            ++i;
        }
    }
    return {geq_1, geq_2};
}


template<>
void for_each_tuple_index<1>(
    const NoveltyBase &novelty_base,
    AtomIndices atom_indices,
    const std::function<bool(TupleIndex)>& callback) {
    int place_holder = novelty_base.get_num_atoms();
    atom_indices.push_back(place_holder);
    for (int atom_index : atom_indices) {
        bool finish = callback(atom_index);
        if (finish) return;
    }
}

template<>
void for_each_tuple_index<2>(
    const NoveltyBase &novelty_base,
    AtomIndices atom_indices,
    const std::function<bool(TupleIndex)>& callback) {
    const std::vector<int>& factors = novelty_base.get_factors();
    int place_holder = novelty_base.get_num_atoms();
    atom_indices.push_back(place_holder);
    int num_atoms = static_cast<int>(atom_indices.size());
    for (int i0 = 0; i0 < num_atoms; ++i0) {
        int i0_tuple_index = factors[0] * atom_indices[i0];
        for (int i1 = (i0 < num_atoms - 1) ? i0 + 1 : i0; i1 < num_atoms; ++i1) {
            TupleIndex tuple_index = i0_tuple_index + factors[1] * atom_indices[i1];
            bool finish = callback(tuple_index);
            if (finish) return;
        }
    }
}


template<>
void for_each_tuple_index<1>(
    const NoveltyBase &novelty_base,
    AtomIndices,
    AtomIndices add_atom_indices,
    const std::function<bool(TupleIndex)>& callback) {
    assert(novelty_base.get_arity() == 1);
    for (int atom_index : add_atom_indices) {
        bool finish = callback(atom_index);
        if (finish) return;
    }
}


template<>
void for_each_tuple_index<2>(
    const NoveltyBase &novelty_base,
    AtomIndices atom_indices,
    AtomIndices add_atom_indices,
    const std::function<bool(TupleIndex)>& callback) {
    assert(novelty_base.get_arity() == 2);
    assert(std::is_sorted(atom_indices.begin(), atom_indices.end()));
    assert(std::is_sorted(add_atom_indices.begin(), add_atom_indices.end()));
    const std::vector<int>& factors = novelty_base.get_factors();
    // Add placeholders to be able to not pick an atom from atom_indices.
    int place_holder = novelty_base.get_num_atoms();
    atom_indices.push_back(place_holder);
    int num_atom_indices = static_cast<int>(atom_indices.size());
    int num_add_atom_indices = static_cast<int>(add_atom_indices.size());
    // Initialize book-keeping for efficient sorted iteration.
    std::array<std::vector<int>, 2> a_geq = compute_geq_mappings(atom_indices, add_atom_indices);
    std::array<AtomIndices, 2> a_atom_indices{std::move(atom_indices), std::move(add_atom_indices)};
    std::array<int, 2> a_num_atom_indices{num_atom_indices, num_add_atom_indices};
    std::array<int, 2> a;
    // Iteration that selects at least one atom index from add_atom_indices.
    for (int k = 1; k < 4; ++k) {
        int tmp = k;
        for (int i = 0; i < 2; ++i) {
            a[i] = (tmp & 1) > 0;
            tmp >>= 1;
        }
        for (int i0 = 0; i0 < a_num_atom_indices[a[0]]; ++i0) {
            int i0_tuple_index = factors[0] * a_atom_indices[a[0]][i0];
            for (int i1 = (a[0] != a[1]) ? a_geq[a[0]][i0] : i0+1; i1 < a_num_atom_indices[a[1]]; ++i1) {
                TupleIndex tuple_index = i0_tuple_index + factors[1] * a_atom_indices[a[1]][i1];
                bool finish = callback(tuple_index);
                if (finish) return;
            }
        }
    }
}

}