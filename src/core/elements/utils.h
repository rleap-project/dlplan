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

extern std::pair<RoleDenotation, std::vector<int>> remap_role_denotation_indices(const RoleDenotation& r_vec, int num_objects);

extern AdjList compute_adjacency_list(const RoleDenotation& r_vec, int num_objects);

extern Distances bfs(const AdjList& adj_list, int source);

extern PairwiseDistances compute_pairwise_distances(const AdjList& adj_list);

}
}
}
}

#endif
