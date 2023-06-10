#include "tuple_index_generator.h"

#include "../utils/logging.h"


namespace dlplan::novelty {


template<>
void for_each_tuple_index<1>(
    const NoveltyBase &novelty_base,
    AtomIndices atom_indices,
    const std::function<bool(int)>& callback) {
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
    const std::function<bool(int)>& callback) {
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
    const std::function<bool(int)>& callback) {
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
    const std::function<bool(int)>& callback) {
    assert(novelty_base.get_arity() == 2);
    assert(std::is_sorted(atom_indices.begin(), atom_indices.end()));
    assert(std::is_sorted(add_atom_indices.begin(), add_atom_indices.end()));
    // Add placeholders to be able to not pick an atom from atom_indices.
    int place_holder = novelty_base.get_num_atoms();
    atom_indices.push_back(place_holder);
    // Initialize book-keeping for efficient sorted iteration.
    std::vector<int> geq_indices(atom_indices.size(), std::numeric_limits<int>::max());
    std::vector<int> geq_indices_add(add_atom_indices.size(), std::numeric_limits<int>::max());
    int j = 0;
    int i = 0;
    while (j < static_cast<int>(geq_indices.size()) && i < static_cast<int>(geq_indices_add.size())) {
        if (atom_indices[j] < add_atom_indices[i]) {
            geq_indices[j] = i;
            ++j;
        } else if (atom_indices[j] > add_atom_indices[i]) {
            geq_indices_add[i] = j;
            ++i;
        } else {
            geq_indices_add[i] = j;
            geq_indices[j] = i;
            ++j;
            ++i;
        }
    }
    // Iteration that selects at least one atom index from add_atom_indices.
    AtomIndices atom_tuple_indices(2);
    std::vector<AtomIndices> a_indices{std::move(atom_indices), std::move(add_atom_indices)};
    std::vector<std::vector<int>> a_geq{std::move(geq_indices), std::move(geq_indices_add)};
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