#ifndef DLPLAN_INCLUDE_DLPLAN_TUPLE_GRAPH_UTILS_H_
#define DLPLAN_INCLUDE_DLPLAN_TUPLE_GRAPH_UTILS_H_


#include "../../include/dlplan/novelty.h"

namespace dlplan::novelty {

/// @brief Creates a canonical representation of the tuple graph
///        where all attributes follow a unique sorting strategie.
///        The primary use of this function for testing purposes.
/// @param tuple_graph
/// @return
extern TupleGraph canonize(const TupleGraph& tuple_graph);

}

#endif
