#include "../../include/dlplan/novelty.h"

#include <cassert>


namespace dlplan::novelty {

/**
 * TupleGraphBuilder separates the construction of a tuple graph
 * to clean up the tuple graph interface.
 */
class TupleGraphBuilder {
private:

public:
    TupleGraph make_tuple_graph(
        std::shared_ptr<const NoveltyBase> novelty_base,
        const state_space::StateSpace& state_space,
        state_space::StateIndex root_state,
        int width,
        bool stop_if_goal) const;
};

static void add_tuple_node(
    TupleNode&& tuple_node,
    int distance,
    std::vector<TupleNodes>& tuple_nodes_by_distance,
    std::vector<StateIndices>& state_indices_by_distance) {
    //state_indices_by_distance.resize(distance+1);
    //tuple_nodes_by_distance.resize(distance+1);
    //tuple_nodes_by_distance[distance].insert(std::move(tuple_node));
}


static void add_edge(
    TupleIndex source,
    TupleIndex target,
    AdjacencyList& forward_successors,
    AdjacencyList& backward_successors) {
    //forward_successors[source].insert(target);
    //backward_successors[target].insert(source);
}


TupleGraph::TupleGraph(
    std::shared_ptr<const NoveltyBase> novelty_base,
    const state_space::StateSpace& state_space,
    state_space::StateIndex root_state,
    int width,
    bool stop_if_goal)
    : m_root_state_index(root_state) {
    assert(width >= 0);
    NoveltyTable novelty_table(novelty_base);
    auto state_information = state_space.compute_state_information();
    auto distances_from_root_state = state_space.compute_distances({root_state}, true, stop_if_goal);
    std::vector<StateIndices> state_indices_by_distance;
    for (const auto& pair : distances_from_root_state) {
        state_indices_by_distance.resize(pair.second + 1);
        state_indices_by_distance[pair.second].push_back(pair.first);
    }
    // initialize root state with distance = 0
    m_tuple_nodes_by_distance.resize(1);
    m_state_indices_by_distance.resize(1);
    for (auto tuple_index : TupleIndexGenerator(novelty_base, state_information.get_state_ref(root_state).get_atom_idxs_ref())) {
        m_tuple_nodes_by_distance[0].push_back(TupleNode(tuple_index, StateIndices{root_state}));
    }
    m_state_indices_by_distance[0].push_back(root_state);
    // iterate distances > 0
    for (int distance = 1; distance < static_cast<int>(state_indices_by_distance.size()); ++distance) {
        // Compute novel tuples and reset their novelty.
        std::unordered_map<StateIndex, TupleIndices> state_index_to_novel_tuples;
        TupleIndicesSet novel_tuples;
        for (auto state_index : state_indices_by_distance[distance]) {
            TupleIndices state_novel_tuples = novelty_table.compute_novel_tuple_indices(TupleIndexGenerator(novelty_base, state_information.get_state_ref(state_index).get_atom_idxs_ref()));
            novel_tuples.insert(state_novel_tuples.begin(), state_novel_tuples.end());
            state_index_to_novel_tuples.emplace(state_index, state_novel_tuples);
        }
        novelty_table.reset_novelty(novel_tuples);
        // Extend optimal plans
        TupleIndicesSet marked_novel_tuples;
        std::unordered_map<std::pair<TupleIndex, TupleIndex>, StateIndicesSet> extended;
        for (const auto& tuple_node : m_tuple_nodes_by_distance[distance - 1]) {
            for (auto state_index : tuple_node.get_state_indices_ref()) {
                for (auto successor_index : state_space.get_forward_successor_state_indices_ref(state_index)) {
                    for (auto tuple_index : state_index_to_novel_tuples.find(successor_index)->second) {
                        extended[std::make_pair(tuple_node.get_tuple_index(), tuple_index)].insert(successor_index);
                    }
                }
            }
            for (auto tuple_index : novel_tuples) {
                if (extended.find(std::make_pair(tuple_node.get_tuple_index(), tuple_index)) == tuple_node.get_state_indices_ref().size()) {
                    marked_novel_tuples.insert(tuple_index);
                }
            }
        }

        m_tuple_nodes_by_distance.resize(distance + 1);
        m_state_indices_by_distance.resize(distance + 1);
        ++distance;
    }
}

TupleGraph::TupleGraph(const TupleGraph& other) = default;

TupleGraph& TupleGraph::operator=(const TupleGraph& other) = default;

TupleGraph::TupleGraph(TupleGraph&& other) = default;

TupleGraph& TupleGraph::operator=(TupleGraph&& other) = default;

TupleGraph::~TupleGraph() = default;

}