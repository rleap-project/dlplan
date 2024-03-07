#include "../../include/dlplan/novelty.h"

#include "tuple_graph_builder.h"
#include "tuple_index_generator.h"
#include "../utils/logging.h"

#include <cassert>
#include <sstream>

using namespace dlplan::state_space;


namespace dlplan::novelty {

TupleGraph::TupleGraph(
    std::shared_ptr<const NoveltyBase> novelty_base,
    std::shared_ptr<const state_space::StateSpace> state_space,
    StateIndex root_state_index)
    : m_novelty_base(novelty_base),
      m_state_space(state_space),
      m_root_state_index(root_state_index) {
    if (!m_novelty_base) {
        throw std::runtime_error("TupleGraph::TupleGraph - novelty_base is nullptr.");
    }
    if (!m_novelty_base) {
        throw std::runtime_error("TupleGraph::TupleGraph - state_space is nullptr.");
    }
    TupleGraphBuilderResult result = TupleGraphBuilder(novelty_base, state_space, root_state_index).get_result();
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
    // Step 1: Sort tuple nodes by tuple index, compute index remapping
    TupleNodes nodes = m_nodes;
    std::sort(nodes.begin(), nodes.end(), [](const TupleNode& l, const TupleNode& r){
        return l.get_tuple_index() < r.get_tuple_index();
    });
    // Remap node indices to get a canonical representation
    std::vector<TupleNodeIndex> remapping(nodes.size());
    for (size_t i = 0; i < nodes.size(); ++i) {
        remapping[nodes[i].get_index()] = i;  // old to new index
    }
    TupleNodes new_nodes;
    new_nodes.reserve(nodes.size());
    for (size_t i = 0; i < nodes.size(); ++i) {
        auto& node = nodes[i];
        TupleNode new_node = TupleNode(i, node.get_tuple_index(), node.get_state_indices());
        for (int pre : node.get_predecessors()) {
            new_node.add_predecessor(remapping[pre]);
        }
        for (int suc : node.get_successors()) {
            new_node.add_successor(remapping[suc]);
        }
        new_nodes.push_back(std::move(new_node));
    }
    std::vector<TupleNodeIndices> new_node_indices_by_distance;
    new_node_indices_by_distance.reserve(m_node_indices_by_distance.size());
    for (const auto& layer : m_node_indices_by_distance) {
        TupleNodeIndices new_layer;
        new_layer.reserve(layer.size());
        for (int node_index : layer) {
            new_layer.push_back(remapping[node_index]);
        }
        std::sort(new_layer.begin(), new_layer.end());
        new_node_indices_by_distance.push_back(std::move(new_layer));
    }
    std::vector<state_space::StateIndices> sorted_state_indices_by_distance = m_state_indices_by_distance;
    std::for_each(sorted_state_indices_by_distance.begin(), sorted_state_indices_by_distance.end(),
              [&](state_space::StateIndices& vec) { std::sort(vec.begin(), vec.end()); });

    std::stringstream ss;

    ss << "TupleGraph(\n"
       << "  root_state_index=" << m_root_state_index << ",\n"
       << "  tuple_nodes=[";
    for (size_t i = 0; i < new_nodes.size(); ++i) {
        if (i > 0) ss << ",\n";
            ss << "    " << new_nodes[i];
    }
    ss << "\n  ],\n"
       << "  node_indices_by_distance=[\n";
    for (size_t i = 0; i < new_node_indices_by_distance.size(); ++i) {
        if (i > 0) ss << ",\n";
        ss << "    " << new_node_indices_by_distance[i];
    }
    ss << "\n  ],\n"
       << "  state_indices_by_distance=[\n";
    for (size_t i = 0; i < sorted_state_indices_by_distance.size(); ++i) {
        if (i > 0) ss << ",\n";
        ss << "    " << sorted_state_indices_by_distance[i];
    }
    ss << "\n  ]\n"
       << ")";
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
            result << "index=" << tuple_node.get_index() << "<BR/>";
            result << "tuple index=" << tuple_node.get_tuple_index() << "<BR/>";
            result << "atoms={";
            const auto atom_indices = m_novelty_base->tuple_index_to_atom_indices(tuple_node.get_tuple_index());
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
            size_t i = 0;
            for (const auto& state_index : tuple_node.get_state_indices()) {
                if (i != 0) {
                    result << ",";
                    if (verbosity_level >= 1) {
                        result << "<BR/>";
                    }
                }
                if (verbosity_level >= 1) {
                    result << m_state_space->get_states().at(state_index).str();
                } else {
                    result << state_index;
                }
                ++i;
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

