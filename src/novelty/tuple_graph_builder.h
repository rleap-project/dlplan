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
    // additional output
    TupleNodes m_nodes;
    std::vector<TupleNodeIndices> m_node_indices_by_distance;
    std::vector<state_space::StateIndices> m_state_indices_by_distance;
    // temporary objects
    std::unordered_map<StateIndex, TupleIndices> m_state_index_to_novel_tuple_indices;
    std::unordered_map<TupleIndex, StateIndices> m_novel_tuple_index_to_state_indices;

public:
    TupleGraphBuilder(
        std::shared_ptr<const NoveltyBase> novelty_base,
        std::shared_ptr<const state_space::StateSpace> state_space,
        StateIndex root_state);

    TupleGraphBuilderResult get_result();
};

}

#endif
