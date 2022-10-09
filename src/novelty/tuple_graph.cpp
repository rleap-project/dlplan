#include "../../include/dlplan/novelty.h"

#include "../../include/dlplan/utils/hashing.h"

#include <cassert>

using namespace dlplan::state_space;


namespace dlplan::novelty {

StateIndices compute_next_layer(
    const StateIndices& current_layer,
    const StateSpace& state_space,
    StateIndicesSet& visited) {
    StateIndicesSet layer_set;
    for (const auto source_index : current_layer) {
        assert(visited.count(source_index));
        for (const auto target_index : state_space.get_forward_successor_state_indices_ref(source_index)) {
            if (!visited.count(target_index)) {
                visited.insert(target_index);
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
    int width)
    : m_root_state_index(root_state) {
    if (width < 0) {
        throw std::runtime_error("TupleGraph::TupleGraph - width must be greater than or equal to 0.");
    }
    /* If width is 0 then we compute a tuple graph for width 1
       and terminate after 1 step in the iteration.
       Each successor of the initial state makes it into this tuple graph
       because each successor has at least one novel tuple of size 1.
    */
    bool zero_width = false;
    if (width == 0) {
        zero_width = true;
        width = 1;
    }
    NoveltyTable novelty_table(novelty_base->get_num_tuples());
    const auto state_information = state_space.compute_state_information();
    StateIndicesSet visited_states;
    // 1. Initialize root state with distance = 0
    StateIndices initial_state_layer{root_state};
    TupleNodes initial_tuple_layer;
    for (auto tuple_index : TupleIndexGenerator(novelty_base, state_information.get_state_ref(root_state).get_atom_idxs_ref())) {
        initial_tuple_layer.push_back(TupleNode(tuple_index, StateIndices{root_state}));
    }
    m_tuple_nodes_by_distance.push_back(initial_tuple_layer);
    m_state_indices_by_distance.push_back(initial_state_layer);
    visited_states.insert(root_state);
    // 2. Iterate distances > 0
    for (int distance = 1; ; ++distance) {
        const StateIndices& prev_state_layer = m_state_indices_by_distance[distance-1];
        const TupleNodes& prev_tuple_layer = m_tuple_nodes_by_distance[distance-1];
        const StateIndices curr_state_layer = compute_next_layer(prev_state_layer, state_space, visited_states);
        TupleNodes curr_tuple_layer;
        // 2.1. Compute novel tuples and reset their novelty.
        std::unordered_map<StateIndex, TupleIndices> state_index_to_novel_tuples;
        std::unordered_map<TupleIndex, StateIndices> novel_tuple_to_state_indices;
        TupleIndicesSet novel_tuples;
        for (const auto state_index : curr_state_layer) {
            const TupleIndices state_novel_tuples = novelty_table.compute_novel_tuple_indices(
                TupleIndexGenerator(
                    novelty_base,
                    state_information.get_state_ref(state_index).get_atom_idxs_ref()));
            novel_tuples.insert(state_novel_tuples.begin(), state_novel_tuples.end());
            state_index_to_novel_tuples.emplace(state_index, state_novel_tuples);
            for (const auto tuple_index : state_novel_tuples) {
                novel_tuple_to_state_indices[tuple_index].push_back(state_index);
            }
        }
        novelty_table.reset_novelty(novel_tuples);
        // 2.2. Extend optimal plans
        std::unordered_map<TupleIndex, TupleNode> curr_tuple_nodes;
        for (auto& tuple_node : m_tuple_nodes_by_distance[distance - 1]) {
            std::unordered_map<TupleIndex, StateIndicesSet> extended;
            for (const auto source_index : tuple_node.get_state_indices_ref()) {
                for (const auto target_index : state_space.get_forward_successor_state_indices_ref(source_index)) {
                    for (const auto target_tuple_index : state_index_to_novel_tuples.find(target_index)->second) {
                        extended[target_tuple_index].insert(source_index);
                    }
                }
            }
            for (const auto& pair : extended) {
                if (pair.second.size() == tuple_node.get_state_indices_ref().size()) {
                    TupleIndex succ_tuple_index = pair.first;
                    auto find = curr_tuple_nodes.find(succ_tuple_index);
                    if (find == curr_tuple_nodes.end()) {
                        curr_tuple_nodes.emplace(succ_tuple_index, TupleNode(succ_tuple_index, std::move(novel_tuple_to_state_indices.at(succ_tuple_index))));
                    }
                    tuple_node.add_successor(succ_tuple_index);
                    curr_tuple_nodes.at(succ_tuple_index).add_predecessor(tuple_node.get_tuple_index());
                }
            }
        }
        for (auto& pair : curr_tuple_nodes) {
            curr_tuple_layer.push_back(std::move(pair.second));
        }
        if (curr_tuple_layer.empty()) {
            break;
        }
        m_tuple_nodes_by_distance.push_back(curr_tuple_layer);
        m_state_indices_by_distance.push_back(curr_state_layer);
        if (zero_width) {
            break;
        }
        ++distance;
    }
}

TupleGraph::TupleGraph(const TupleGraph& other) = default;

TupleGraph& TupleGraph::operator=(const TupleGraph& other) = default;

TupleGraph::TupleGraph(TupleGraph&& other) = default;

TupleGraph& TupleGraph::operator=(TupleGraph&& other) = default;

TupleGraph::~TupleGraph() = default;

const std::vector<TupleNodes>& TupleGraph::get_tuple_nodes_by_distance_ref() const {
    return m_tuple_nodes_by_distance;
}

const std::vector<state_space::StateIndices>& TupleGraph::get_state_indices_by_distance_ref() const {
    return m_state_indices_by_distance;
}

state_space::StateIndex TupleGraph::get_root_state_index() const {
    return m_root_state_index;
}

int TupleGraph::get_width() const {
    return m_width;
}

}