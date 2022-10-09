#include "../../include/dlplan/novelty.h"

#include "../../include/dlplan/utils/hashing.h"

#include <cassert>

using namespace dlplan::state_space;


namespace dlplan::novelty {

StateIndices compute_next_layer(
    const StateIndices& current_layer,
    const StateSpace& state_space,
    Distances& distances) {
    StateIndicesSet layer_set;
    for (auto source_index : current_layer) {
        assert(distances.count(source_index));
        int source_cost = distances[source_index];
        for (auto target_index : state_space.get_forward_successor_state_indices_ref(source_index)) {
            if (!distances.count(target_index)) {
                distances[target_index] = source_cost + 1;
                layer_set.insert(target_index);
            }
        }
    }
    return StateIndices(layer_set.begin(), layer_set.end());
}

TupleGraph::TupleGraph(
    std::shared_ptr<const NoveltyBase> novelty_base,
    const StateSpace& state_space,
    StateIndex root_state,
    int width,
    bool stop_if_goal)
    : m_root_state_index(root_state) {
    assert(width >= 0);
    NoveltyTable novelty_table(novelty_base->get_num_tuples());
    auto state_information = state_space.compute_state_information();
    Distances distances;
    // 1. Initialize root state with distance = 0
    m_tuple_nodes_by_distance.resize(1);
    m_state_indices_by_distance.resize(1);
    for (auto tuple_index : TupleIndexGenerator(novelty_base, state_information.get_state_ref(root_state).get_atom_idxs_ref())) {
        m_tuple_nodes_by_distance[0].push_back(TupleNode(tuple_index, StateIndices{root_state}));
    }
    m_state_indices_by_distance[0].push_back(root_state);
    distances[root_state] = 0;
    // 2. Iterate distances > 0
    for (int distance = 1; ; ++distance) {
        const StateIndices& prev_state_layer = m_state_indices_by_distance[distance-1];
        StateIndices curr_state_layer = compute_next_layer(prev_state_layer, state_space, distances);
        TupleNodes& prev_tuple_layer = m_tuple_nodes_by_distance[distance-1];
        TupleNodes curr_tuple_layer;
        // Compute novel tuples and reset their novelty.
        std::unordered_map<StateIndex, TupleIndices> state_index_to_novel_tuples;
        std::unordered_map<TupleIndex, StateIndices> novel_tuple_to_state_indices;
        TupleIndicesSet novel_tuples;
        for (auto state_index : curr_state_layer) {
            TupleIndices state_novel_tuples = novelty_table.compute_novel_tuple_indices(TupleIndexGenerator(novelty_base, state_information.get_state_ref(state_index).get_atom_idxs_ref()));
            novel_tuples.insert(state_novel_tuples.begin(), state_novel_tuples.end());
            state_index_to_novel_tuples.emplace(state_index, state_novel_tuples);
            for (auto tuple_index : state_novel_tuples) {
                novel_tuple_to_state_indices[tuple_index].push_back(state_index);
            }
        }
        novelty_table.reset_novelty(novel_tuples);
        // Extend optimal plans
        for (auto& tuple_node : m_tuple_nodes_by_distance[distance - 1]) {
            std::unordered_map<TupleIndex, StateIndicesSet> extended;
            for (auto source_index : tuple_node.get_state_indices_ref()) {
                for (auto target_index : state_space.get_forward_successor_state_indices_ref(source_index)) {
                    for (auto target_tuple_index : state_index_to_novel_tuples.find(target_index)->second) {
                        extended[target_tuple_index].insert(source_index);
                    }
                }
            }
            for (const auto& pair : extended) {
                if (pair.second.size() == tuple_node.get_state_indices_ref().size()) {
                    TupleIndex succ_tuple_index = pair.first;
                    TupleNode succ_tuple_node(succ_tuple_index, novel_tuple_to_state_indices.at(succ_tuple_index));
                    tuple_node.add_successor(succ_tuple_index);
                    succ_tuple_node.add_predecessor(tuple_node.get_tuple_index());
                    curr_tuple_layer.push_back(std::move(succ_tuple_node));
                }
            }
        }
        if (curr_tuple_layer.empty()) {
            break;
        }
        m_tuple_nodes_by_distance.push_back(curr_tuple_layer);
        m_state_indices_by_distance.push_back(curr_state_layer);
        ++distance;
    }
}

TupleGraph::TupleGraph(const TupleGraph& other) = default;

TupleGraph& TupleGraph::operator=(const TupleGraph& other) = default;

TupleGraph::TupleGraph(TupleGraph&& other) = default;

TupleGraph& TupleGraph::operator=(TupleGraph&& other) = default;

TupleGraph::~TupleGraph() = default;

}