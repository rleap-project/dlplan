#ifndef DLPLAN_SRC_CORE_ELEMENTS_UTILS_H_
#define DLPLAN_SRC_CORE_ELEMENTS_UTILS_H_

#include "types.h"


namespace dlplan::core {
class RoleDenotation;
namespace element::utils {

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
extern AdjList compute_adjacency_list(const RoleDenotation& r);

/**
 * Compute distances from a given state in a graph defined by an adjacency list.
 */
extern Distances compute_distances_from_state(const AdjList& adj_list, int source);

/**
 * Compute pairwise distances with floyd warshall algorithm.
 */
extern PairwiseDistances compute_floyd_warshall(const AdjList& adj_list, bool reflexive=true);

/**
 * Compute the transitive (reflexive) closure over the role denotation
 */
extern RoleDenotation compute_transitive_closure(const PairwiseDistances& distances, int num_objects);

}
}

#endif
