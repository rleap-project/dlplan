#include "../../include/dlplan/novelty.h"

#include <cassert>


namespace dlplan::novelty {

TupleGraph::TupleGraph(
    std::shared_ptr<const NoveltyBase> novelty_base,
    const state_space::StateSpace& state_space,
    state_space::StateIndex root_state,
    int width,
    bool stop_if_goal)
    : m_root_state_index(root_state) {
    assert(width >= 0);
    NoveltyTable novelty_table(novelty_base);
    // Initialize root state.
    // For compactness, we store a single tuple index proof for novelty
    // but we mark all tuple indices as not novel.
    m_tuple_nodes_by_distance.resize(1);
    m_state_indices_by_distance.resize(1);
    m_tuple_nodes_by_distance[0].emplace_back(root_state, StateIndices{root_state});
    m_state_indices_by_distance[0].emplace_back(root_state);
    if (stop_if_goal && state_space.is_goal(root_state)) {
        return;
    }
    if (width == 0) {
        // The initial state and every all its successors with distance 1 are novel.
        // Hence, we simply use the successor state indices as their representative novel tuple.
        for (StateIndex successor : state_space.get_forward_successor_state_indices_ref(root_state)) {
            m_tuple_nodes_by_distance.resize(2);
            m_state_indices_by_distance.resize(2);
            m_tuple_nodes_by_distance[1].emplace_back(successor, StateIndices{successor});
            m_state_indices_by_distance[1].emplace_back(successor);
        }
    } else {
        // Compute the tuple graph according to standard definition.
        auto distances = state_space.compute_distances({root_state}, true, stop_if_goal);
    }
}

TupleGraph::TupleGraph(const TupleGraph& other) = default;

TupleGraph& TupleGraph::operator=(const TupleGraph& other) = default;

TupleGraph::TupleGraph(TupleGraph&& other) = default;

TupleGraph& TupleGraph::operator=(TupleGraph&& other) = default;

TupleGraph::~TupleGraph() = default;

}