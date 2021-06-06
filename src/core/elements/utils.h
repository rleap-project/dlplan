#ifndef DLP_SRC_CORE_ELEMENTS_UTILS_H_
#define DLP_SRC_CORE_ELEMENTS_UTILS_H_

#include "types.h"


namespace dlp {
namespace core {
namespace element {
namespace utils {

using AdjList = std::vector<std::vector<int>>;
using Distances = std::vector<int>;
using PairwiseDistances = std::vector<Distances>;

/**
 * Path addition evaluations to INF if any term is INF.
 */
extern int path_addition(int a, int b);

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
