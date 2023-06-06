#include "../../include/dlplan/novelty.h"

#include "tuple_index_generator.h"

#include "../utils/logging.h"

#include <cassert>
#include <sstream>

using namespace dlplan::state_space;


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


TupleGraph::TupleGraph(
    std::shared_ptr<const NoveltyBase> novelty_base,
    std::shared_ptr<const state_space::StateSpace> state_space,
    StateIndex root_state)
    : m_novelty_base(novelty_base),
      m_state_space(state_space),
      m_root_state_index(root_state) {
    if (!novelty_base) {
        throw std::runtime_error("TupleGraph::TupleGraph - novelty_base is nullptr.");
    }
    if (!state_space) {
        throw std::runtime_error("TupleGraph::TupleGraph - state_space is nullptr.");
    }
    if (novelty_base->get_arity() == 0) {
        TupleNodeIndex tuple_node_index = m_nodes.size();
        TupleIndex tuple_index = m_novelty_base->atom_tuple_to_tuple_index({});
        m_node_indices_by_distance.push_back({tuple_node_index});
        m_nodes.push_back({TupleNode(tuple_node_index, tuple_index, {root_state})});
        m_state_indices_by_distance.push_back({root_state});
        const auto& it = state_space->get_forward_successor_state_indices().find(root_state);
        if (it != state_space->get_forward_successor_state_indices().end()) {
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
        NoveltyTable novelty_table(novelty_base);
        StateIndicesSet visited_states;
        m_state_indices_by_distance.push_back(StateIndices{root_state});
        TupleNodeIndices initial_tuple_layer;
        TupleIndices tuple_indices = novelty_table.compute_novel_tuple_indices(state_space->get_states().at(root_state).get_atom_indices(), {});
        for (const auto tuple_index : tuple_indices) {
            int tuple_node_index = m_nodes.size();
            m_nodes.push_back(TupleNode(tuple_node_index, tuple_index, StateIndices{root_state}));
            initial_tuple_layer.push_back(tuple_node_index);
        }
        m_node_indices_by_distance.push_back(std::move(initial_tuple_layer));
        novelty_table.insert(tuple_indices, false);
        visited_states.insert(root_state);
        // 2. Iterate distances > 0
        for (int distance = 1; ; ++distance) {
            // 2.1. Compute unique states in curr state layer.
            StateIndices curr_state_layer = compute_state_layer(
                m_state_indices_by_distance[distance-1],
                *state_space,
                visited_states);
            // 2.2. Compute novel tuples, mappings between states, and reset their novelty.
            auto [novel_tuples,
                state_index_to_novel_tuple_indices,
                novel_tuple_index_to_state_indices] = compute_novel_tuple_indices_layer(
                    curr_state_layer,
                    *state_space,
                    novelty_table);
            if (novel_tuples.empty()) {
                break;
            }
            // 2.3. Extend optimal plans of tuples from previous layer to tuples in current layer
            TupleNodeIndices curr_tuple_layer = compute_tuple_nodes_layer(
                m_nodes,
                m_node_indices_by_distance[distance-1],
                *state_space,
                state_index_to_novel_tuple_indices,
                novel_tuple_index_to_state_indices);
            if (curr_tuple_layer.empty()) {
                break;
            }
            m_node_indices_by_distance.push_back(std::move(curr_tuple_layer));
            m_state_indices_by_distance.push_back(std::move(curr_state_layer));
        }
    }
}

TupleGraph::TupleGraph(const TupleGraph& other) = default;

TupleGraph& TupleGraph::operator=(const TupleGraph& other) = default;

TupleGraph::TupleGraph(TupleGraph&& other) = default;

TupleGraph& TupleGraph::operator=(TupleGraph&& other) = default;

TupleGraph::~TupleGraph() = default;

std::string TupleGraph::compute_repr() const {
    std::stringstream ss;
    /*
    auto cmp = [](const TupleNode& l, const TupleNode& r){ return l.get_tuple_index() < r.get_tuple_index(); };
    std::vector<TupleNodeIndices> sorted_tuple_node_indices_by_distance = m_node_indices_by_distance;
    std::for_each(sorted_tuple_node_indices_by_distance.begin(), sorted_tuple_node_indices_by_distance.end(),
              [&](std::vector<TupleNode>& vec) { std::sort(vec.begin(), vec.end()); });
    std::vector<state_space::StateIndices> sorted_state_indices_by_distance = m_state_indices_by_distance;
    std::for_each(sorted_state_indices_by_distance.begin(), sorted_state_indices_by_distance.end(),
              [&](state_space::StateIndices& vec) { std::sort(vec.begin(), vec.end()); });
    ss << "TupleGraph(\n"
       << "  root_state_index=" << m_root_state_index << ",\n"
       << "  tuple_nodes_by_distance=[\n";
    for (const auto& tuple_nodes : sorted_tuple_node_indices_by_distance) {
        ss << "    [\n";
        for (size_t i = 0; i < tuple_nodes.size(); ++i) {
            if (i > 0) ss << ",\n";
            ss << "      " << tuple_nodes[i];
        }
        ss << "\n    ],\n";
    }
    ss << "  ],\n"
       << "  state_indices_by_distance=[\n";
    for (size_t i = 0; i < sorted_state_indices_by_distance.size(); ++i) {
        if (i > 0) ss << ",\n";
        ss << "    " << sorted_state_indices_by_distance[i];
    }
    ss << "\n  ]\n"
       << ")";
       */
    ss << "asd";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const TupleGraph& tuple_graph) {
    os << tuple_graph.compute_repr();
    return os;
}

std::string TupleGraph::str() const {
    return compute_repr();
}

std::string TupleGraph::to_dot(int verbosity_level) const {
    std::stringstream result;
    // 2. Header
    result << "digraph {" << "\n"
           << "rankdir=\"LR\"" << "\n";
    // 3. Tuple nodes.
    for (int node_index : m_node_indices_by_distance.front()) {
        result << "Dangling" << node_index << " [ label = \"\", style = invis ]\n";
    }
    const auto& atoms = m_state_space->get_instance_info()->get_atoms();
    for (const auto& tuple_layer : m_node_indices_by_distance) {
        for (int node_index : tuple_layer) {
            const auto& tuple_node = m_nodes[node_index];
            result << "t" << tuple_node.get_index() << "[";
            result << "label=<";
            result << "tuple index=" << tuple_node.get_tuple_index() << "<BR/>";
            result << "atoms={";
            const auto atom_indices = m_novelty_base->tuple_index_to_atom_tuple(tuple_node.get_tuple_index());
            for (size_t i = 0; i < atom_indices.size(); ++i) {
                if (i != 0) {
                    result << ",";
                }
                if (verbosity_level >= 1) {
                    result << atoms[atom_indices[i]].get_name();
                } else {
                    result << atom_indices[i];
                }
            }
            result << "}<BR/>";
            result << "states={";
            const auto& state_indices = tuple_node.get_state_indices();
            for (size_t i = 0; i < state_indices.size(); ++i) {
                if (i != 0) {
                    result << ",";
                    if (verbosity_level >= 1) {
                        result << "<BR/>";
                    }
                }
                if (verbosity_level >= 1) {
                    result << m_state_space->get_states().at(state_indices[i]).str();
                } else {
                    result << state_indices[i];
                }
            }
            result << "}>]\n";
        }
    }
    // 4. Group states with same distance together
    // 5. Tuple edges
    result << "{\n";
    for (int node_index : m_node_indices_by_distance.front()) {
        const auto& tuple_node = m_nodes[node_index];
        result << "Dangling" << tuple_node.get_index() << "->t" << tuple_node.get_index() << "\n";
    }
    result << "}\n";
    for (const auto& tuple_layer : m_node_indices_by_distance) {
        result << "{\n";
        for (int node_index : tuple_layer) {
            for (int succ_node_index : m_nodes[node_index].get_successors()) {
                result << "t" << node_index << "->" << "t" << succ_node_index << "\n";
            }
        }
        result << "}\n";
    }
    result << "}\n";
    return result.str();
}

std::shared_ptr<const NoveltyBase> TupleGraph::get_novelty_base() const {
    return m_novelty_base;
}

std::shared_ptr<const state_space::StateSpace> TupleGraph::get_state_space() const {
    return m_state_space;
}

state_space::StateIndex TupleGraph::get_root_state_index() const {
    return m_root_state_index;
}

const TupleNodes& TupleGraph::get_tuple_nodes() const {
    return m_nodes;
}

const std::vector<TupleNodeIndices>& TupleGraph::get_tuple_node_indices_by_distance() const {
    return m_node_indices_by_distance;
}

const std::vector<state_space::StateIndices>& TupleGraph::get_state_indices_by_distance() const {
    return m_state_indices_by_distance;
}

}