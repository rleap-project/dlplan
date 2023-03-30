#include "../../include/dlplan/novelty.h"

#include "../utils/logging.h"

#include <sstream>

using namespace dlplan::state_space;


namespace dlplan::novelty {

TupleNode::TupleNode(TupleIndex tuple_index, const StateIndices& state_indices)
    : m_tuple_index(tuple_index), m_state_indices(state_indices) { }

TupleNode::TupleNode(TupleIndex tuple_index, StateIndices&& state_indices)
    : m_tuple_index(tuple_index), m_state_indices(std::move(state_indices)) { }

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

std::string TupleNode::str() const {
    std::stringstream result;
    result << "(" << m_tuple_index << ", " << m_state_indices << ")";
    return result.str();
}

TupleIndex TupleNode::get_tuple_index() const {
    return m_tuple_index;
}

const StateIndices& TupleNode::get_state_indices() const {
    return m_state_indices;
}

const TupleIndices& TupleNode::get_predecessors() const {
    return m_predecessors;
}

const TupleIndices& TupleNode::get_successors() const {
    return m_successors;
}

}