#include "tuple_graph_builder.h"


namespace dlplan::novelty {

StateIndices TupleGraphBuilder::compute_state_layer(
    const StateIndices& current_layer,
    StateIndicesSet &visited_state_indices) {
    std::unordered_set<StateIndex> layer_set;
    const auto& successors = m_state_space->get_forward_successor_state_indices();
    for (const auto source_index : current_layer) {
        assert(visited_state_indices.count(source_index));
        if (successors.count(source_index)) {
            for (const auto target_index : successors.at(source_index)) {
                if (!visited_state_indices.count(target_index)) {
                    visited_state_indices.insert(target_index);
                    layer_set.insert(target_index);
                }
            }
        }
    }
    return StateIndices(layer_set.begin(), layer_set.end());
}

TupleIndices TupleGraphBuilder::compute_novel_tuple_indices_layer(const StateIndices& curr_state_layer) {
    std::unordered_set<TupleIndex> novel_tuples_set;
    for (const auto state_index : curr_state_layer) {
        const TupleIndices state_novel_tuples = m_novelty_table.compute_novel_tuple_indices(
            {},
            m_state_space->get_states().at(state_index).get_atom_indices());
        novel_tuples_set.insert(state_novel_tuples.begin(), state_novel_tuples.end());
        m_state_index_to_novel_tuple_indices.emplace(state_index, state_novel_tuples);
        for (const auto tuple_index : state_novel_tuples) {
            m_novel_tuple_index_to_state_indices[tuple_index].push_back(state_index);
        }
    }
    TupleIndices novel_tuples = TupleIndices(novel_tuples_set.begin(), novel_tuples_set.end());
    m_novelty_table.insert_tuple_indices(novel_tuples, false);
    return novel_tuples;
}

std::unordered_map<TupleIndex, StateIndicesSet>
TupleGraphBuilder::extend_states(TupleIndex cur_node_index) const {
    std::unordered_map<TupleIndex, StateIndicesSet> extended;
    const auto& successors = m_state_space->get_forward_successor_state_indices();
    for (const auto source_index : m_nodes[cur_node_index].get_state_indices()) {
        if (successors.count(source_index)) {
            for (const auto target_index : successors.at(source_index)) {
                if (m_state_index_to_novel_tuple_indices.count(target_index)) {
                    for (const auto target_tuple_index : m_state_index_to_novel_tuple_indices.find(target_index)->second) {
                        extended[target_tuple_index].insert(source_index);
                    }
                }
            }
        }
    }
    return extended;
}

void TupleGraphBuilder::extend_nodes(
    TupleNodeIndex cur_node_index,
    std::unordered_map<TupleIndex, TupleNodeIndex> &novel_tuple_index_to_node) {
    auto extended = extend_states(cur_node_index);
    for (const auto& pair : extended) {
        if (pair.second.size() == m_nodes[cur_node_index].get_state_indices().size()) {
            int succ_node_index = m_nodes.size();
            TupleIndex succ_tuple_index = pair.first;
            auto find = novel_tuple_index_to_node.find(succ_tuple_index);
            if (find == novel_tuple_index_to_node.end()) {
                novel_tuple_index_to_node.emplace(succ_tuple_index, succ_node_index);
                m_nodes.push_back(TupleNode(
                    succ_node_index,
                    succ_tuple_index,
                    m_novel_tuple_index_to_state_indices.at(succ_tuple_index)));
            } else {
                succ_node_index = find->second;
            }
            m_nodes[cur_node_index].add_successor(succ_node_index);
            m_nodes[succ_node_index].add_predecessor(cur_node_index);
        }
    }
}

TupleNodeIndices TupleGraphBuilder::compute_nodes_layer(TupleNodeIndices& prev_tuple_layer) {
    TupleNodeIndices curr_tuple_layer;
    std::unordered_map<TupleIndex, TupleNodeIndex> novel_tuple_index_to_node;
    for (auto& cur_node_index : prev_tuple_layer) {
        extend_nodes(cur_node_index, novel_tuple_index_to_node);
    }
    for (auto& pair : novel_tuple_index_to_node) {
        curr_tuple_layer.push_back(std::move(pair.second));
    }
    return curr_tuple_layer;
}

void TupleGraphBuilder::build_width_equal_0_tuple_graph() {
    TupleNodeIndex node_index = m_nodes.size();
    TupleIndex tuple_index = m_novelty_base->atom_indices_to_tuple_index({});
    m_node_indices_by_distance.push_back({node_index});
    m_nodes.push_back({TupleNode(node_index, tuple_index, {m_root_state_index})});
    m_state_indices_by_distance.push_back({m_root_state_index});
    const auto& it = m_state_space->get_forward_successor_state_indices().find(m_root_state_index);
    if (it != m_state_space->get_forward_successor_state_indices().end()) {
        TupleNodeIndices curr_tuple_layer;
        StateIndices curr_state_layer;
        for (const auto& target_index : it->second) {
            node_index = m_nodes.size();
            curr_tuple_layer.push_back(node_index);
            m_nodes.push_back(TupleNode(node_index, tuple_index, {target_index}));
            curr_state_layer.push_back(target_index);
        }
        m_node_indices_by_distance.push_back(curr_tuple_layer);
        m_state_indices_by_distance.push_back(curr_state_layer);
    }
}

void TupleGraphBuilder::build_width_greater_0_tuple_graph() {
    StateIndicesSet visited_state_indices;
    // 1. Initialize root state with distance = 0
    m_state_indices_by_distance.push_back(StateIndices{m_root_state_index});
    TupleNodeIndices initial_tuple_layer;
    TupleIndices tuple_indices = m_novelty_table.compute_novel_tuple_indices(m_state_space->get_states().at(m_root_state_index).get_atom_indices());
    for (const auto tuple_index : tuple_indices) {
        int node_index = m_nodes.size();
        m_nodes.push_back(TupleNode(node_index, tuple_index, StateIndices{m_root_state_index}));
        initial_tuple_layer.push_back(node_index);
    }
    m_node_indices_by_distance.push_back(std::move(initial_tuple_layer));
    m_novelty_table.insert_tuple_indices(tuple_indices, false);
    visited_state_indices.insert(m_root_state_index);
    // 2. Iterate distances > 0
    for (int distance = 1; ; ++distance) {
        StateIndices curr_state_layer = compute_state_layer(m_state_indices_by_distance[distance-1], visited_state_indices);
        auto novel_tuples = compute_novel_tuple_indices_layer(curr_state_layer);
        if (novel_tuples.empty()) {
            break;
        }
        TupleNodeIndices curr_tuple_layer = compute_nodes_layer(m_node_indices_by_distance[distance-1]);
        if (curr_tuple_layer.empty()) {
            break;
        }
        m_node_indices_by_distance.push_back(std::move(curr_tuple_layer));
        m_state_indices_by_distance.push_back(std::move(curr_state_layer));
    }
}

TupleGraphBuilder::TupleGraphBuilder(
    std::shared_ptr<const NoveltyBase> novelty_base,
    std::shared_ptr<const state_space::StateSpace> state_space,
    StateIndex root_state)
    : m_novelty_base(novelty_base),
      m_state_space(state_space),
      m_root_state_index(root_state),
      m_novelty_table(novelty_base) {
    if (!m_novelty_base) {
        throw std::runtime_error("TupleGraphBuilder::TupleGraphBuilder - novelty_base is nullptr.");
    }
    if (!m_novelty_base) {
        throw std::runtime_error("TupleGraphBuilder::TupleGraphBuilder - state_space is nullptr.");
    }
    if (m_novelty_base->get_arity() == 0) {
        build_width_equal_0_tuple_graph();
    } else {
        build_width_greater_0_tuple_graph();
    }
}

TupleGraphBuilderResult TupleGraphBuilder::get_result() {
    return TupleGraphBuilderResult{
        std::move(m_nodes),
        std::move(m_node_indices_by_distance),
        std::move(m_state_indices_by_distance)
    };
}

}