#ifndef DLPLAN_INCLUDE_DLPLAN_TUPLE_INDEX_GENERATOR_H_
#define DLPLAN_INCLUDE_DLPLAN_TUPLE_INDEX_GENERATOR_H_

#include "../../include/dlplan/novelty.h"

#include "../utils/logging.h"


namespace dlplan::novelty {

/// @brief Computes mappings from current index i in sorted vector v
///        to smallest index i' in sorted vector v' such that
///        v[i] < v[j].
///
/// Example: vec_1 = [1,3,4], vec_2 = [2,5]
///          geq_1 = [0,1,1], geq_2 = [1,inf]
/// @param vec_1
/// @param vec_2
/// @return
extern std::array<std::vector<int>, 2> compute_geq_mappings(
    const AtomIndices& vec_1,
    const AtomIndices& vec_2);


/// @brief
/// @tparam Arity
/// @param novelty_base
/// @param atom_indices
/// @param callback
template<int Arity=-1>
void for_each_tuple_index(
    const NoveltyBase &novelty_base,
    AtomIndices atom_indices,
    const std::function<bool(TupleIndex)>& callback) {
    int arity = novelty_base.get_arity();
    // Add placeholders to be able to generate tuples of size less than arity.
    int place_holder = novelty_base.get_num_atoms();
    atom_indices.push_back(place_holder);
    int num_atom_indices = static_cast<int>(atom_indices.size());
    // Initialize iteration indices.
    std::vector<int> indices(arity);
    AtomIndices atom_tuple_indices(arity);
    for (int i = 0; i < arity; ++i) {
        int index = std::min(i, num_atom_indices);
        indices[i] = index;
        atom_tuple_indices[i] = atom_indices[index];
    }
    TupleIndex tuple_index = novelty_base.atom_indices_to_tuple_index(atom_tuple_indices);
    /*
        This iteration has amortized time O(1). Computing tuple index from atom tuple has time O(arity).
        Can be improved to O(1) as well by only considering the flipped values.
    */
    while (true) {
        bool finished = callback(tuple_index);
        if (finished) return;
        // Find the rightmost index to increment
        int i = arity - 1;
        while (i >= 0 && (indices[i] >= num_atom_indices - 1)) {
            --i;
        }
        if (i < 0) {
            // Exit the loop when all indices have reached their maximum values
            return;
        }
        int index = ++indices[i];
        atom_tuple_indices[i] = atom_indices[index];
        // Update indices right of the incremented rightmost index i.
        for (int j = i + 1; j < arity; ++j) {
            // Ensure that index is not larger than index of place holder.
            int index = std::min(num_atom_indices - 1, indices[j-1] + 1);
            indices[j] = index;
            atom_tuple_indices[j] = atom_indices[index];
        }
        tuple_index = novelty_base.atom_indices_to_tuple_index(atom_tuple_indices);
    }
}

/// @brief Spezialized implementation with constant depth 1 for-loop
///        and direct computation of hash value.
/// @param novelty_base
/// @param atom_indices
/// @param callback
template<>
void for_each_tuple_index<1>(
    const NoveltyBase &novelty_base,
    AtomIndices atom_indices,
    const std::function<bool(TupleIndex)>& callback);

/// @brief Spezialized implementation with constant depth 2 for-loop
///        and direct computation of hash value.
/// @param novelty_base
/// @param atom_indices
/// @param callback
template<>
void for_each_tuple_index<2>(
    const NoveltyBase &novelty_base,
    AtomIndices atom_indices,
    const std::function<bool(TupleIndex)>& callback);


/// @brief
/// @tparam Arity
/// @param novelty_base
/// @param atom_indices
/// @param add_atom_indices
/// @param callback
template<int Arity=-1>
void for_each_tuple_index(
    const NoveltyBase& novelty_base,
    AtomIndices atom_indices,
    AtomIndices add_atom_indices,
    const std::function<bool(TupleIndex)>& callback) {
    int arity = novelty_base.get_arity();
    assert(std::is_sorted(atom_indices.begin(), atom_indices.end()));
    assert(std::is_sorted(add_atom_indices.begin(), add_atom_indices.end()));
    // Add placeholders to be able to not pick an atom from atom_indices.
    int place_holder = novelty_base.get_num_atoms();
    atom_indices.push_back(place_holder);
    int num_atom_indices = static_cast<int>(atom_indices.size());
    int num_add_atom_indices = static_cast<int>(add_atom_indices.size());
    // Initialize book-keeping for efficient sorted iteration.
    std::array<std::vector<int>, 2> a_geq = compute_geq_mappings(atom_indices, add_atom_indices);
    std::array<AtomIndices, 2> a_atom_indices{std::move(atom_indices), std::move(add_atom_indices)};
    std::array<int, 2> a_num_atom_indices{num_atom_indices, num_add_atom_indices};
    // Iteration that selects at least one atom index from add_atom_indices.
    AtomIndices atom_tuple_indices(arity);
    std::vector<int> a(arity);
    std::vector<int> indices(arity);
    for (int k = 1; k < std::pow(2, arity); ++k) {
        int tmp = k;
        for (int i = 0; i < arity; ++i) {
            a[i] = (tmp & 1) > 0;
            tmp >>= 1;
        }
        // Initialize iteration indices.
        indices[0] = 0;
        atom_tuple_indices[0] = a_atom_indices[a[0]][0];
        bool exhausted = false;
        for (int i = 1; i < arity; ++i) {
            int index = indices[i] = (a[i-1] != a[i]) ? a_geq[a[i-1]][indices[i-1]] : indices[i-1] + 1;
            if (index == std::numeric_limits<int>::max()) {
                // no larger atom index can be appended.
                exhausted = true;
                break;
            }
            if (a[i-1] == a[i] && a[i] == 1 && indices[i-1] == indices[i]) {
                // duplicate atom indices from add_atom_indices
                exhausted = true;
                break;
            }
            atom_tuple_indices[i] = a_atom_indices[a[i]][indices[i]];
        }
        if (exhausted) {
            continue;
        }
        TupleIndex tuple_index = novelty_base.atom_indices_to_tuple_index(atom_tuple_indices);
        /*
          This iteration has amortized time O(1). Computing tuple index from atom tuple has time O(arity).
          Can be improved to O(1) as well by only considering the flipped values.
        */
        while (true) {
            bool finished = callback(tuple_index);
            if (finished) return;
            // Find the rightmost index to increment
            int i = arity - 1;
            while (i >= 0 && (indices[i] >= a_num_atom_indices[a[i]] - 1)) {
                --i;
            }
            if (i < 0) {
                // Exit the loop when all indices have reached their maximum values
                break;
            }
            int index = ++indices[i];
            atom_tuple_indices[i] = a_atom_indices[a[i]][index];
            // Update indices right of the incremented rightmost index i.
            bool exhausted = false;
            for (int j = i + 1; j < arity; ++j) {
                int index = indices[j] = (a[j-1] != a[j]) ? a_geq[a[j-1]][indices[j-1]] : std::min(a_num_atom_indices[a[j]] - 1, indices[j-1] + 1);
                if (index == std::numeric_limits<int>::max()) {
                    // no larger atom index can be appended.
                    exhausted = true;
                    break;
                }
                indices[j] = index;
                if (a[j-1] == a[j] && a[j] == 1 && indices[j-1] == indices[j]) {
                    // duplicate atom indices from add_atom_indices
                    exhausted = true;
                    break;
                }
                atom_tuple_indices[j] = a_atom_indices[a[j]][indices[j]];
            }
            if (exhausted) {
                break;
            }
            tuple_index = novelty_base.atom_indices_to_tuple_index(atom_tuple_indices);
        }
    }
}

/// @brief Spezialized implementation with constant depth 1 for-loop
///        and direct computation of hash value.
/// @param novelty_base
/// @param atom_indices
/// @param add_atom_indices
/// @param callback
template<>
void for_each_tuple_index<1>(
    const NoveltyBase& novelty_base,
    AtomIndices atom_indices,
    AtomIndices add_atom_indices,
    const std::function<bool(TupleIndex)>& callback);


/// @brief Spezialized implementation with constant depth 2 for-loop
///        and direct computation of hash value.
/// @param novelty_base
/// @param atom_indices
/// @param add_atom_indices
/// @param callback
template<>
void for_each_tuple_index<2>(
    const NoveltyBase& novelty_base,
    AtomIndices atom_indices,
    AtomIndices add_atom_indices,
    const std::function<bool(TupleIndex)>& callback);

}

#endif
