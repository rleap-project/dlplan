#include "tuple_graph_builder.h"


namespace dlplan::novelty {

static StateIndices compute_state_layer(
    const StateIndices& current_layer,
    const StateSpace& state_space,
    StateIndicesSet& visited) {
    std::unordered_set<StateIndex> layer_set;
    const auto& successors = state_space.get_forward_successor_state_indices();
    for (const auto source_index : current_layer) {
        assert(visited.count(source_index));
        if (successors.count(source_index)) {
            for (const auto target_index : successors.at(source_index)) {
                if (!visited.count(target_index)) {
                    visited.insert(target_index);
                    layer_set.insert(target_index);
                }
            }
        }
    }
    return StateIndices(layer_set.begin(), layer_set.end());
}


static std::tuple<TupleIndices, std::unordered_map<StateIndex, TupleIndices>, std::unordered_map<TupleIndex, StateIndices>>
compute_novel_tuple_indices_layer(
    const StateIndices& curr_state_layer,
    const StateSpace& state_space,
    NoveltyTable& novelty_table) {
    std::unordered_map<StateIndex, TupleIndices> state_index_to_novel_tuple_indices;
    std::unordered_map<TupleIndex, StateIndices> novel_tuple_index_to_state_indices;
    std::unordered_set<TupleIndex> novel_tuples_set;
    AtomIndices effect_atom_indices;
    for (const auto state_index : curr_state_layer) {
        const TupleIndices state_novel_tuples = novelty_table.compute_novel_tuple_indices(
            state_space.get_states().at(state_index).get_atom_indices(),
            effect_atom_indices);
        novel_tuples_set.insert(state_novel_tuples.begin(), state_novel_tuples.end());
        state_index_to_novel_tuple_indices.emplace(state_index, state_novel_tuples);
        for (const auto tuple_index : state_novel_tuples) {
            novel_tuple_index_to_state_indices[tuple_index].push_back(state_index);
        }
    }
    TupleIndices novel_tuples = TupleIndices(novel_tuples_set.begin(), novel_tuples_set.end());
    novelty_table.reset_novelty(novel_tuples);
    return std::tuple<TupleIndices, std::unordered_map<StateIndex, TupleIndices>, std::unordered_map<TupleIndex, StateIndices>>{novel_tuples, state_index_to_novel_tuple_indices, novel_tuple_index_to_state_indices};
}


static std::unordered_map<TupleIndex, StateIndicesSet>
extend_tuple_node(
    const TupleNode& tuple_node,
    const StateSpace& state_space,
    const std::unordered_map<StateIndex, TupleIndices>& state_index_to_novel_tuple_indices) {
    std::unordered_map<TupleIndex, StateIndicesSet> extended;
    const auto& successors = state_space.get_forward_successor_state_indices();
    for (const auto source_index : tuple_node.get_state_indices()) {
        if (successors.count(source_index)) {
            for (const auto target_index : successors.at(source_index)) {
                if (state_index_to_novel_tuple_indices.count(target_index)) {
                    for (const auto target_tuple_index : state_index_to_novel_tuple_indices.find(target_index)->second) {
                        extended[target_tuple_index].insert(source_index);
                    }
                }
            }
        }
    }
    return extended;
}


static void
add_extended_tuple_nodes(
    const std::unordered_map<TupleIndex, StateIndicesSet>& extended,
    TupleNodes& tuple_nodes,
    TupleNodeIndex cur_node_index,
    std::unordered_map<TupleIndex, TupleNodeIndex>& novel_tuple_index_to_node,
    std::unordered_map<TupleIndex, StateIndices>& novel_tuple_index_to_state_indices) {
    for (const auto& pair : extended) {
        if (pair.second.size() == tuple_nodes[cur_node_index].get_state_indices().size()) {
            int succ_node_index = tuple_nodes.size();
            TupleIndex succ_tuple_index = pair.first;
            auto find = novel_tuple_index_to_node.find(succ_tuple_index);
            if (find == novel_tuple_index_to_node.end()) {
                novel_tuple_index_to_node.emplace(succ_tuple_index, succ_node_index);
                tuple_nodes.push_back(TupleNode(
                    succ_node_index,
                    succ_tuple_index,
                    novel_tuple_index_to_state_indices.at(succ_tuple_index)));
            } else {
                succ_node_index = find->second;
            }
            tuple_nodes[cur_node_index].add_successor(succ_node_index);
            tuple_nodes[succ_node_index].add_predecessor(cur_node_index);
        }
    }
}


static TupleNodeIndices
compute_tuple_nodes_layer(
    TupleNodes& tuple_nodes,
    TupleNodeIndices& prev_tuple_layer,
    const StateSpace& state_space,
    const std::unordered_map<StateIndex, TupleIndices>& state_index_to_novel_tuple_indices,
    std::unordered_map<TupleIndex, StateIndices>& novel_tuple_index_to_state_indices) {
    TupleNodeIndices curr_tuple_layer;
    std::unordered_map<TupleIndex, TupleNodeIndex> novel_tuple_index_to_node;
    for (auto& cur_node_index : prev_tuple_layer) {
        std::unordered_map<TupleIndex, StateIndicesSet> extended = extend_tuple_node(
            tuple_nodes[cur_node_index],
            state_space,
            state_index_to_novel_tuple_indices);
        add_extended_tuple_nodes(
            extended,
            tuple_nodes,
            cur_node_index,
            novel_tuple_index_to_node,
            novel_tuple_index_to_state_indices);
    }
    for (auto& pair : novel_tuple_index_to_node) {
        curr_tuple_layer.push_back(std::move(pair.second));
    }
    return curr_tuple_layer;
}

TupleGraphBuilder::TupleGraphBuilder(
    std::shared_ptr<const NoveltyBase> novelty_base,
    std::shared_ptr<const state_space::StateSpace> state_space,
    StateIndex root_state)
    : m_novelty_base(novelty_base),
      m_state_space(state_space),
      m_root_state_index(root_state) {
}

TupleGraphBuilderResult TupleGraphBuilder::get_result() {
    if (!m_novelty_base) {
        throw std::runtime_error("TupleGraph::TupleGraph - novelty_base is nullptr.");
    }
    if (!m_novelty_base) {
        throw std::runtime_error("TupleGraph::TupleGraph - state_space is nullptr.");
    }
    if (m_novelty_base->get_arity() == 0) {
        TupleNodeIndex tuple_node_index = m_nodes.size();
        TupleIndex tuple_index = m_novelty_base->atom_tuple_to_tuple_index({});
        m_node_indices_by_distance.push_back({tuple_node_index});
        m_nodes.push_back({TupleNode(tuple_node_index, tuple_index, {m_root_state_index})});
        m_state_indices_by_distance.push_back({m_root_state_index});
        const auto& it = m_state_space->get_forward_successor_state_indices().find(m_root_state_index);
        if (it != m_state_space->get_forward_successor_state_indices().end()) {
            TupleNodeIndices curr_tuple_layer;
            StateIndices curr_state_layer;
            for (const auto& target_index : it->second) {
                tuple_node_index = m_nodes.size();
                curr_tuple_layer.push_back(tuple_node_index);
                m_nodes.push_back(TupleNode(tuple_node_index, tuple_index, {target_index}));
                curr_state_layer.push_back(target_index);
            }
            m_node_indices_by_distance.push_back(curr_tuple_layer);
            m_state_indices_by_distance.push_back(curr_state_layer);
        }
    } else {
        // 1. Initialize root state with distance = 0
        NoveltyTable novelty_table(m_novelty_base);
        StateIndicesSet visited_states;
        m_state_indices_by_distance.push_back(StateIndices{m_root_state_index});
        TupleNodeIndices initial_tuple_layer;
        TupleIndices tuple_indices = novelty_table.compute_novel_tuple_indices(m_state_space->get_states().at(m_root_state_index).get_atom_indices(), {});
        for (const auto tuple_index : tuple_indices) {
            int tuple_node_index = m_nodes.size();
            m_nodes.push_back(TupleNode(tuple_node_index, tuple_index, StateIndices{m_root_state_index}));
            initial_tuple_layer.push_back(tuple_node_index);
        }
        m_node_indices_by_distance.push_back(std::move(initial_tuple_layer));
        novelty_table.insert(tuple_indices, false);
        visited_states.insert(m_root_state_index);
        // 2. Iterate distances > 0
        for (int distance = 1; ; ++distance) {
            // 2.1. Compute unique states in curr state layer.
            StateIndices curr_state_layer = compute_state_layer(
                m_state_indices_by_distance[distance-1],
                *m_state_space,
                visited_states);
            // 2.2. Compute novel tuples, mappings between states, and reset their novelty.
            auto [novel_tuples,
                state_index_to_novel_tuple_indices,
                novel_tuple_index_to_state_indices] = compute_novel_tuple_indices_layer(
                    curr_state_layer,
                    *m_state_space,
                    novelty_table);
            if (novel_tuples.empty()) {
                break;
            }
            // 2.3. Extend optimal plans of tuples from previous layer to tuples in current layer
            TupleNodeIndices curr_tuple_layer = compute_tuple_nodes_layer(
                m_nodes,
                m_node_indices_by_distance[distance-1],
                *m_state_space,
                state_index_to_novel_tuple_indices,
                novel_tuple_index_to_state_indices);
            if (curr_tuple_layer.empty()) {
                break;
            }
            m_node_indices_by_distance.push_back(std::move(curr_tuple_layer));
            m_state_indices_by_distance.push_back(std::move(curr_state_layer));
        }
    }
    return TupleGraphBuilderResult{std::move(m_nodes), std::move(m_node_indices_by_distance), std::move(m_state_indices_by_distance)};
}

}