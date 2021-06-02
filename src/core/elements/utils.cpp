#include "utils.h"

#include <deque>
#include <iostream>


namespace dlp {
namespace core {
namespace element {
namespace utils {

std::pair<RoleDenotation, std::vector<int>> remap_role_denotation_indices(const RoleDenotation& r_vec, int num_objects) {
    ConceptDenotation_Set c_set;
    for (const auto& r : r_vec) {
        c_set.insert(r.first);
        c_set.insert(r.second);
    }
    ConceptDenotation c_vec(c_set.begin(), c_set.end());
    std::vector<int> encode(num_objects, UNDEFINED);
    std::vector<int> decode(c_vec.size());
    for (int i = 0; i < c_vec.size(); ++i) {
        encode[c_vec[i]] = i;
        decode[i] = c_vec[i];
    }
    RoleDenotation r_vec_new;
    for (const auto& r : r_vec) {
        r_vec_new.push_back(std::make_pair(encode[r.first], encode[r.second]));
    }
    return std::make_pair(r_vec_new, decode);
}

AdjList compute_adjacency_list(const RoleDenotation& r_vec, int num_objects) {
    AdjList adjacency_list(num_objects);
    for (const auto& r : r_vec) {
        adjacency_list[r.first].push_back(r.second);
    }
    return adjacency_list;
}

Distances compute_distances_from_state(const AdjList& adj_list, int source) {
    Distances distances(adj_list.size(), INF);
    std::vector<bool> visited(adj_list.size(), false);
    distances[source] = 0;
    visited[source] = true;
    std::deque<int> queue;
    queue.push_back(source);
    while (!queue.empty()) {
        int source = queue.front();
        queue.pop_front();
        for (int target : adj_list[source]) {
            if (!visited[target]) {
                int alt = distances[source] + 1;
                if (distances[target] > alt) {
                    distances[target] = alt;
                }
                visited[target] = true;
                queue.push_back(target);
            }
        }
    }
    return distances;
}

PairwiseDistances compute_pairwise_distances(const AdjList& adj_list) {
    PairwiseDistances pairwise_distances;
    for (int source = 0; source < adj_list.size(); ++source) {
        pairwise_distances.emplace_back(compute_distances_from_state(adj_list, source));
    }
    return pairwise_distances;
}

}
}
}
}
