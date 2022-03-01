#ifndef DLPLAN_SRC_CORE_ELEMENTS_UTILS_H_
#define DLPLAN_SRC_CORE_ELEMENTS_UTILS_H_

#include "types.h"

#include "../../../include/dlplan/core.h"


namespace dlplan::core::element::utils {

using Distances = std::vector<int>;
using PairwiseDistances = std::vector<Distances>;

extern int path_addition(int a, int b);

extern int compute_multi_source_multi_target_shortest_distance(const ConceptDenotation& sources, const RoleDenotation& edges, const ConceptDenotation& targets);

extern Distances compute_multi_source_multi_target_shortest_distances(const ConceptDenotation& sources, const RoleDenotation& edges, const ConceptDenotation& targets);

extern PairwiseDistances compute_floyd_warshall(const RoleDenotation& edges);

extern dynamic_bitset::DynamicBitset<unsigned> concept_denot_to_bitset(const ConceptDenotation& denot);

extern dynamic_bitset::DynamicBitset<unsigned> role_denot_to_bitset(const RoleDenotation& denot);

extern RoleDenotation bitset_to_role_denotation(dynamic_bitset::DynamicBitset<unsigned> bitset, int num_objects);

}

#endif
