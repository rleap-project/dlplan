#include "utils.h"

#include <deque>
#include <iostream>

#include "../types.h"
#include "../../../include/dlplan/core.h"


namespace dlplan::core::element::utils {

using AdjList = std::vector<std::vector<int>>;

int path_addition(int a, int b) {
    if (a == INF || b == INF) {
        return INF;
    } else {
        if (a > 0 && b > INF - a) {
            // handle overflow
            return INF;
        }
        return a + b;
    }
}


AdjList compute_adjacency_list(const RoleDenotation& role_denot) {
    int num_objects = role_denot.get_num_objects();
    AdjList adjacency_list(num_objects);
    for (const auto& pair : role_denot) {
        adjacency_list[pair.first].push_back(pair.second);
    }
    return adjacency_list;
}


Distances compute_distances_from_state(const AdjList& adj_list, int source) {
    Distances distances(adj_list.size(), INF);
    distances[source] = 0;
    std::deque<int> queue;
    queue.push_back(source);
    while (!queue.empty()) {
        int s = queue.front();
        queue.pop_front();
        for (int t : adj_list[s]) {
            int alt = distances[s] + 1;
            if (distances[t] > alt) {
                queue.push_back(t);
                distances[t] = alt;
            }
        }
    }
    return distances;
}


int compute_multi_source_multi_target_shortest_distance(const ConceptDenotation& sources, const RoleDenotation& edges, const ConceptDenotation& targets) {
    int num_objects = targets.get_num_objects();
    AdjList adj_list = compute_adjacency_list(edges);
    Distances distances(num_objects, INF);
    std::deque<int> queue;
    for (int i = 0; i < num_objects; ++i) {
        if (sources.count(i)) {
            distances[i] = 0;
            queue.push_back(i);
        }
    }
    while (!queue.empty()) {
        int s = queue.front();
        queue.pop_front();
        for (int t : adj_list[s]) {
            int alt = distances[s] + 1;
            if (distances[t] > alt) {
                if (targets.count(t)) {
                    return alt;
                }
                queue.push_back(t);
                distances[t] = alt;
            }
        }
    }
    return INF;
}

int compute_single_source_multi_target_shortest_distance(int source, const RoleDenotation& edges, const ConceptDenotation& targets) {
    ConceptDenotation source_dummy(targets.get_num_objects());
    source_dummy.insert(source);
    return compute_multi_source_multi_target_shortest_distance(source_dummy, edges, targets);
}


PairwiseDistances compute_floyd_warshall(const RoleDenotation& edges) {
    int num_objects = edges.get_num_objects();
    AdjList adj_list = compute_adjacency_list(edges);
    PairwiseDistances dist(num_objects, Distances(num_objects, INF));
    // initialize edge costs
    for (int source = 0; source < num_objects; ++source) {
        for (int target : adj_list[source]) {
            dist[source][target] = 1;
        }
        dist[source][source] = 0;
    }
    // main loop
    for (int k = 0; k < num_objects; ++k) {
        for (int i = 0; i < num_objects; ++i) {
            for (int j = 0; j < num_objects; ++j) {
                if (dist[i][j] > path_addition(dist[i][k], dist[k][j])) {
                    dist[i][j] = path_addition(dist[i][k], dist[k][j]);
                }
            }
        }
    }
    return dist;
}


dynamic_bitset::DynamicBitset<unsigned> concept_denot_to_bitset(const ConceptDenotation& denot) {
    dynamic_bitset::DynamicBitset<unsigned> result(denot.get_num_objects());
    for (const auto single : denot) {
        result.set(single);
    }
    return result;
}


dynamic_bitset::DynamicBitset<unsigned>role_denot_to_bitset(const RoleDenotation& denot) {
    int num_objects = denot.get_num_objects();
    dynamic_bitset::DynamicBitset<unsigned> result(num_objects * num_objects);
    for (const auto& pair : denot) {
        result.set(pair.first * num_objects + pair.second);
    }
    return result;
}

RoleDenotation bitset_to_role_denotation(dynamic_bitset::DynamicBitset<unsigned> bitset, int num_objects) {
    RoleDenotation role_denot(num_objects);
    int offset = 0;
    for (int i = 0; i < num_objects; ++i) {
        for (int j = 0; j < num_objects; ++j) {
            if (bitset.test(offset + j)) {
                role_denot.insert(std::make_pair(i, j));
            }
        }
        offset += num_objects;
    }
    return role_denot;
}

}
