#include "utils.h"

#include <deque>
#include <iostream>

#include "../types.h"


namespace dlp {
namespace core {
namespace element {
namespace utils {

extern int path_addition(int a, int b) {
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
        pairwise_distances.push_back(compute_distances_from_state(adj_list, source));
    }
    return pairwise_distances;
}

}
}
}
}
