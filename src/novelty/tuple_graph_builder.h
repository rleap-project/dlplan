#ifndef DLPLAN_INCLUDE_DLPLAN_TUPLE_GRAPH_BUILDER_H_
#define DLPLAN_INCLUDE_DLPLAN_TUPLE_GRAPH_BUILDER_H_

#include "../../include/dlplan/novelty.h"

using namespace dlplan::state_space;


namespace dlplan::novelty {

struct TupleGraphBuilderResult {
    TupleNodes nodes;
    std::vector<TupleNodeIndices> node_indices_by_distance;
    std::vector<state_space::StateIndices> state_indices_by_distance;
};


/// @brief Separates the construction of a tuple graph from its interface.
class TupleGraphBuilder {
private:
    // input
    std::shared_ptr<const NoveltyBase> m_novelty_base;
    std::shared_ptr<const state_space::StateSpace> m_state_space;
    state_space::StateIndex m_root_state_index;
    // output
    TupleNodes m_nodes;
    std::vector<TupleNodeIndices> m_node_indices_by_distance;
    std::vector<state_space::StateIndices> m_state_indices_by_distance;
    // temporary objects
    NoveltyTable m_novelty_table;
    std::unordered_map<StateIndex, TupleIndices> m_state_index_to_novel_tuple_indices;
    std::unordered_map<TupleIndex, StateIndicesSet> m_novel_tuple_index_to_state_indices;

private:
    /// @brief Computes all nodes in next layer, given the current layer and
    ///        states that have been visited in previous layers.
    /// @param current_layer
    /// @param visited_state_indices
    /// @return
    StateIndices
    compute_state_layer(
        const StateIndices& current_layer,
        StateIndicesSet &visited_state_indices);

    /// @brief Computes all tuples that are novel in any state of the given layer.
    /// @param current_state_layer
    TupleIndicesSet
    compute_novel_tuple_indices_layer(
        const StateIndices& curr_state_layer);

    /// @brief Computes all nodes in next layer, given the ones in the current layer.
    TupleNodeIndices
    compute_nodes_layer(
        const TupleIndicesSet& curr_novel_tuple_indices,
        TupleNodeIndices& prev_tuple_layer);

    /// @brief Computes all successor tuples in next layer for the tuple in current layer
    ///        with node index cur_node_index with its respective states that were extended.
    std::unordered_map<TupleIndex, StateIndicesSet>
    extend_states(TupleIndex cur_node_index) const;

    void build_width_equal_0_tuple_graph();

    void build_width_greater_0_tuple_graph();

public:
    TupleGraphBuilder(
        std::shared_ptr<const NoveltyBase> novelty_base,
        std::shared_ptr<const state_space::StateSpace> state_space,
        StateIndex root_state);

    TupleGraphBuilderResult get_result();
};

}

#endif
