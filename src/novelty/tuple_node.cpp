#include "../../include/dlplan/novelty.h"


namespace dlplan::novelty {

TupleNode::TupleNode(TupleIndex tuple_index, StateIndices&& state_indices)
    : m_tuple_index(tuple_index), m_state_indices(std::move(state_indices)) { }

TupleNode::TupleNode(const TupleNode& other) = default;

TupleNode& TupleNode::operator=(const TupleNode& other) = default;

TupleNode::TupleNode(TupleNode&& other) = default;

TupleNode& TupleNode::operator=(TupleNode&& other) = default;

TupleNode::~TupleNode() = default;

}