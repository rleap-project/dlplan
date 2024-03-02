#include "../../include/dlplan/novelty.h"

#include "../utils/logging.h"

#include <sstream>


namespace dlplan::novelty {

TupleNode::TupleNode(TupleNodeIndex index, TupleIndex tuple_index, const StateIndicesSet& state_indices)
    : m_index(index), m_tuple_index(tuple_index), m_state_indices(state_indices) { }

TupleNode::TupleNode(TupleNodeIndex index, TupleIndex tuple_index, StateIndicesSet&& state_indices)
    : m_index(index), m_tuple_index(tuple_index), m_state_indices(std::move(state_indices)) { }

TupleNode::TupleNode(const TupleNode& other) = default;

TupleNode& TupleNode::operator=(const TupleNode& other) = default;

TupleNode::TupleNode(TupleNode&& other) = default;

TupleNode& TupleNode::operator=(TupleNode&& other) = default;

TupleNode::~TupleNode() = default;

void TupleNode::add_predecessor(TupleIndex tuple_index) {
    m_predecessors.push_back(tuple_index);
}

void TupleNode::add_successor(TupleIndex tuple_index) {
    m_successors.push_back(tuple_index);
}

std::string TupleNode::compute_repr() const {
    std::stringstream ss;
    TupleIndices sorted_predecessors(m_predecessors.begin(), m_predecessors.end());
    std::sort(sorted_predecessors.begin(), sorted_predecessors.end());
    TupleIndices sorted_successors(m_successors.begin(), m_successors.end());
    std::sort(sorted_successors.begin(), sorted_successors.end());
    ss << "TupleNode("
       << "index=" << m_index << ", "
       << "tuple_index=" << m_tuple_index << ", "
       << "state_indices=" << m_state_indices << ", "
       << "predecessors=" << sorted_predecessors << ", "
       << "successors=" << sorted_successors
       << ")";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const TupleNode& tuple_node) {
    os << tuple_node.compute_repr();
    return os;
}

std::string TupleNode::str() const {
    std::stringstream result;
    result << "(" << m_tuple_index << ", " << m_state_indices << ")";
    return result.str();
}

TupleNodeIndex TupleNode::get_index() const {
    return m_index;
}

TupleIndex TupleNode::get_tuple_index() const {
    return m_tuple_index;
}

const StateIndicesSet& TupleNode::get_state_indices() const {
    return m_state_indices;
}

const TupleIndices& TupleNode::get_predecessors() const {
    return m_predecessors;
}

const TupleIndices& TupleNode::get_successors() const {
    return m_successors;
}

}
