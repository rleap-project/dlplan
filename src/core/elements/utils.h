#ifndef DLP_SRC_CORE_ELEMENTS_UTILS_H_
#define DLP_SRC_CORE_ELEMENTS_UTILS_H_

#include <vector>
#include <utility>

#include "../types.h"


namespace dlp {
namespace core {
namespace element {
namespace utils {

using AdjList = std::vector<std::vector<int>>;
using Distances = std::vector<int>;
using PairwiseDistances = std::vector<Distances>;

/**
 * Remaps the role denotations from a fragmented index scheme to an index scheme
 * to be able compute a more compact version of an adjacency list.
 */
extern std::pair<RoleDenotation, std::vector<int>> remap_role_denotation_indices(const RoleDenotation& r_vec, int num_objects);

/**
 * Computes an adjacency list from a given role.
 */
extern AdjList compute_adjacency_list(const RoleDenotation& r_vec, int num_objects);

/**
 * Compute distances from a given state in a graph defined by an adjacency list.
 */
extern Distances compute_distances_from_state(const AdjList& adj_list, int source);

/**
 * Compute pairwise distances in a graph defined by an adjacency list.
 */
extern PairwiseDistances compute_pairwise_distances(const AdjList& adj_list);

}
}
}
}

#endif
