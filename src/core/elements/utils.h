#ifndef DLPLAN_SRC_CORE_ELEMENTS_UTILS_H_
#define DLPLAN_SRC_CORE_ELEMENTS_UTILS_H_

#include "types.h"

#include "../../../include/dlplan/core.h"


const int INF = std::numeric_limits<int>::max();

namespace dlplan::core::element::utils {

using Distances = std::vector<int>;
using PairwiseDistances = std::vector<Distances>;

extern int path_addition(int a, int b);

extern int compute_multi_source_multi_target_shortest_distance(const ConceptDenotation& sources, const RoleDenotation& edges, const ConceptDenotation& targets);

extern Distances compute_multi_source_multi_target_shortest_distances(const ConceptDenotation& sources, const RoleDenotation& edges, const ConceptDenotation& targets);

extern PairwiseDistances compute_floyd_warshall(const RoleDenotation& edges);

}

#endif
