#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_UTILS_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_UTILS_H_

#include "../../core.h"


const int SCORE_CONSTANT = 1;
const int SCORE_LINEAR = 100;
const int SCORE_QUADRATIC = SCORE_LINEAR * SCORE_LINEAR;
const int SCORE_QUBIC = SCORE_LINEAR * SCORE_LINEAR * SCORE_LINEAR;

const int INF = std::numeric_limits<int>::max();

namespace dlplan::core::utils {

using Distances = std::vector<int>;
using PairwiseDistances = std::vector<Distances>;

extern int path_addition(int a, int b);

extern int compute_multi_source_multi_target_shortest_distance(const ConceptDenotation& sources, const RoleDenotation& edges, const ConceptDenotation& targets);

extern Distances compute_multi_source_multi_target_shortest_distances(const ConceptDenotation& sources, const RoleDenotation& edges, const ConceptDenotation& targets);

extern PairwiseDistances compute_floyd_warshall(const RoleDenotation& edges);

}

#endif
