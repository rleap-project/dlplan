#include "../../include/dlplan/novelty.h"

#include "tuple_graph_builder.h"
#include "tuple_graph_utils.h"
#include "tuple_index_generator.h"

#include "../utils/logging.h"

#include <cassert>
#include <sstream>

using namespace dlplan::state_space;


namespace dlplan::novelty {

TupleGraph::TupleGraph(
    std::shared_ptr<const NoveltyBase> novelty_base,
    std::shared_ptr<const state_space::StateSpace> state_space,
    StateIndex root_state)
    : m_novelty_base(novelty_base),
      m_state_space(state_space),
      m_root_state_index(root_state) {
    TupleGraphBuilderResult result = TupleGraphBuilder(novelty_base, state_space, root_state).get_result();
    m_nodes = std::move(result.nodes);
    m_node_indices_by_distance = std::move(result.node_indices_by_distance);
    m_state_indices_by_distance = std::move(result.state_indices_by_distance);
}

TupleGraph::TupleGraph(const TupleGraph& other) = default;

TupleGraph& TupleGraph::operator=(const TupleGraph& other) = default;

TupleGraph::TupleGraph(TupleGraph&& other) = default;

TupleGraph& TupleGraph::operator=(TupleGraph&& other) = default;

TupleGraph::~TupleGraph() = default;

std::string TupleGraph::compute_repr() const {
    std::stringstream ss;
    /*
    auto tuple_graph = canonize(*this);
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