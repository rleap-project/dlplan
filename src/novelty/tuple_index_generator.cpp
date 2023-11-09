#include "tuple_index_generator.h"

#include "../utils/logging.h"

#include <cmath>


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


void for_each_tuple_index(
    const NoveltyBase &novelty_base,
    AtomIndices atom_indices,
    const std::function<bool(TupleIndex)>& callback) {
    assert(std::is_sorted(atom_indices.begin(), atom_indices.end()));

    const std::vector<int>& factors = novelty_base.get_factors();
    int arity = novelty_base.get_arity();
    // Add placeholders to be able to generate tuples of size less than arity.
    atom_indices.push_back(NoveltyBase::place_holder);
    std::transform(atom_indices.begin(), atom_indices.end(), atom_indices.begin(), [](int index){ return ++index; });
    std::rotate(atom_indices.rbegin(), atom_indices.rbegin() + 1, atom_indices.rend());
    assert(std::is_sorted(atom_indices.begin(), atom_indices.end()));
    int num_atom_indices = static_cast<int>(atom_indices.size());
    // Initialize iteration indices.
    std::vector<int> indices(arity, 0);
    TupleIndex tuple_index = 0;
    /*
        This iteration has amortized time O(1) to compute the next tuple index.
    */
    while (true) {
        bool finished = callback(tuple_index);
        if (finished) return;
        // Find the rightmost index to increment
        int i = arity - 1;
        while (i >= 0 && (indices[i] >= num_atom_indices - (arity - i))) {
            --i;
        }
        if (i < 0) {
            // Exit the loop when all indices have reached their maximum values
            return;
        }
        int index = ++indices[i];
        tuple_index += factors[i] * (atom_indices[index] - atom_indices[index - 1]);
        // Update indices right of the incremented rightmost index i.
        for (int j = i + 1; j < arity; ++j) {
            // Ensure that index is not larger than index of place holder.
            int old_index = indices[j];
            int new_index = indices[j] = indices[j-1] + 1;
            tuple_index += factors[j] * (atom_indices[new_index] - atom_indices[old_index]);
        }
    }
}


void for_each_tuple_index(
    const NoveltyBase& novelty_base,
    AtomIndices atom_indices,
    AtomIndices add_atom_indices,
    const std::function<bool(TupleIndex)>& callback) {
    assert(std::is_sorted(atom_indices.begin(), atom_indices.end()));
    assert(std::is_sorted(add_atom_indices.begin(), add_atom_indices.end()));

    if (add_atom_indices.empty()) {
        // No tuple index exists.
        return;
    }
    const std::vector<int>& factors = novelty_base.get_factors();
    int arity = novelty_base.get_arity();
    // Add placeholders to be able to not pick an atom from atom_indices.
    atom_indices.push_back(NoveltyBase::place_holder);
    std::transform(atom_indices.begin(), atom_indices.end(), atom_indices.begin(), [](int index){ return ++index; });
    std::rotate(atom_indices.rbegin(), atom_indices.rbegin() + 1, atom_indices.rend());
    assert(std::is_sorted(atom_indices.begin(), atom_indices.end()));
    std::transform(add_atom_indices.begin(), add_atom_indices.end(), add_atom_indices.begin(), [](int index){ return ++index; });
    int num_atom_indices = static_cast<int>(atom_indices.size());
    int num_add_atom_indices = static_cast<int>(add_atom_indices.size());
    // Initialize book-keeping for efficient sorted iteration.
    std::array<std::vector<int>, 2> a_geq = compute_geq_mappings(atom_indices, add_atom_indices);
    std::array<AtomIndices, 2> a_atom_indices{std::move(atom_indices), std::move(add_atom_indices)};
    std::array<int, 2> a_num_atom_indices{num_atom_indices, num_add_atom_indices};
    // Iteration that selects at least one atom index from add_atom_indices.
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
        TupleIndex tuple_index = factors[0] * a_atom_indices[a[0]][0];  // existence of atom index is guaranteed.
        bool exhausted = false;
        // Fill as many leading place holders as possible.
        int i = 1;
        if (a[0] == 0) {
            while (a[i] == 0) {
                indices[i] = 0;
                ++i;
            }
        }
        for (; i < arity; ++i) {
            int index = indices[i] = (a[i-1] != a[i]) ? a_geq[a[i-1]][indices[i-1]] : std::min(a_num_atom_indices[a[i]] - 1, indices[i-1] + 1);
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
            tuple_index += factors[i] * a_atom_indices[a[i]][indices[i]];
        }
        if (exhausted) {
            continue;
        }
        /*
            This iteration has amortized time O(1) to compute the next tuple index.
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
            tuple_index += factors[i] * (a_atom_indices[a[i]][index] - a_atom_indices[a[i]][index - 1]);
            // Update indices right of the incremented rightmost index i.
            bool exhausted = false;
            for (int j = i + 1; j < arity; ++j) {
                int old_index = indices[j];
                int new_index = indices[j] = (a[j-1] != a[j]) ? a_geq[a[j-1]][indices[j-1]] : std::min(a_num_atom_indices[a[j]] - 1, indices[j-1] + 1);
                if (new_index == std::numeric_limits<int>::max()) {
                    // no larger atom index can be appended.
                    exhausted = true;
                    break;
                }
                if (a[j-1] == a[j] && a[j] == 1 && indices[j-1] == indices[j]) {
                    // duplicate atom indices from add_atom_indices
                    exhausted = true;
                    break;
                }
                tuple_index += factors[j] * (a_atom_indices[a[j]][new_index] - a_atom_indices[a[j]][old_index]);
            }
            if (exhausted) {
                break;
            }
        }
    }
}

}