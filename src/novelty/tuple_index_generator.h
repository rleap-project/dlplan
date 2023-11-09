#ifndef DLPLAN_INCLUDE_DLPLAN_TUPLE_INDEX_GENERATOR_H_
#define DLPLAN_INCLUDE_DLPLAN_TUPLE_INDEX_GENERATOR_H_

#include "../utils/logging.h"
#include "../../include/dlplan/novelty.h"

#include <array>
#include <vector>
#include <functional>


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
/// @param novelty_base
/// @param atom_indices A vector of atom indices sorted ascendingly.
/// @param callback
extern void for_each_tuple_index(
    const NoveltyBase &novelty_base,
    AtomIndices atom_indices,
    const std::function<bool(TupleIndex)>& callback);

/// @brief
/// @param novelty_base
/// @param atom_indices A vector of atom indices sorted ascendingly.
/// @param add_atom_indices A vector of atom indices sorted ascendingly.
/// @param callback
extern void for_each_tuple_index(
    const NoveltyBase& novelty_base,
    AtomIndices atom_indices,
    AtomIndices add_atom_indices,
    const std::function<bool(TupleIndex)>& callback);

}

#endif
