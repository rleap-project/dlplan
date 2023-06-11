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
    assert(std::is_sorted(atom_indices.begin(), atom_indices.end()));
    int place_holder = novelty_base.get_num_atoms();
    atom_indices.push_back(place_holder);
    AtomIndices atom_tuple_indices(1);
    for (int atom_index : atom_indices) {
        atom_tuple_indices[0] = atom_index;
        int tuple_index = novelty_base.atom_tuple_to_tuple_index(atom_tuple_indices);
        bool finish = callback(tuple_index);
        if (finish) return;
    }
}

template<>
void for_each_tuple_index<2>(
    const NoveltyBase &novelty_base,
    AtomIndices atom_indices,
    const std::function<bool(TupleIndex)>& callback) {
    int place_holder = novelty_base.get_num_atoms();
    atom_indices.push_back(place_holder);
    AtomIndices atom_tuple_indices(2);
    int num_atoms = static_cast<int>(atom_indices.size());
    for (int i1 = 0; i1 < num_atoms; ++i1) {
        atom_tuple_indices[0] = atom_indices[i1];
        for (int i2 = (i1 < num_atoms - 1) ? i1 + 1 : i1; i2 < num_atoms; ++i2) {
            atom_tuple_indices[1] = atom_indices[i2];
            int tuple_index = novelty_base.atom_tuple_to_tuple_index(atom_tuple_indices);
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
    assert(std::is_sorted(add_atom_indices.begin(), add_atom_indices.end()));
    AtomIndices atom_tuple_indices(1);
    for (int atom_index : add_atom_indices) {
        atom_tuple_indices[0] = atom_index;
        int tuple_index = novelty_base.atom_tuple_to_tuple_index(atom_tuple_indices);
        bool finish = callback(tuple_index);
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
    // Add placeholders to be able to not pick an atom from atom_indices.
    int place_holder = novelty_base.get_num_atoms();
    atom_indices.push_back(place_holder);
    // Initialize book-keeping for efficient sorted iteration.
    std::array<std::vector<int>, 2> a_geq = compute_geq_mappings(atom_indices, add_atom_indices);
    std::array<AtomIndices, 2> a_indices{std::move(atom_indices), std::move(add_atom_indices)};
    // Iteration that selects at least one atom index from add_atom_indices.
    AtomIndices atom_tuple_indices(2);
    for (int k = 1; k < 4; ++k) {
        int a0 = (k & 1) > 0;
        int a1 = (k & 2) > 0;
        const auto& a0_indices = a_indices[a0];
        const auto& a1_indices = a_indices[a1];
        const auto& a0_geq = a_geq[a0];
        int num_a0_indices = static_cast<int>(a0_indices.size());
        int num_a1_indices = static_cast<int>(a1_indices.size());
        for (int i = 0; i < num_a0_indices; ++i) {
            atom_tuple_indices[0] = a0_indices[i];
            for (int j = (a0 != a1) ? a0_geq[i] : i+1; j < num_a1_indices; ++j) {
                atom_tuple_indices[1] = a1_indices[j];
                int tuple_index = novelty_base.atom_tuple_to_tuple_index(atom_tuple_indices);
                bool finish = callback(tuple_index);
                if (finish) return;
            }
        }
    }
}

}