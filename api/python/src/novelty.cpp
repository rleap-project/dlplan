#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/novelty.h"

namespace py = pybind11;

using namespace dlplan::novelty;
using namespace dlplan::state_space;


void init_novelty(py::module_ &m) {
    py::class_<NoveltyBase, std::shared_ptr<NoveltyBase>>(m, "NoveltyBase")
        .def(py::init<int, int>())
        .def("__copy__", [](const NoveltyBase& base, py::object){ return NoveltyBase(base); })
        .def("__deepcopy__", [](const NoveltyBase& base, py::object){ return NoveltyBase(base); })
        .def("atom_tuple_to_tuple_index", &NoveltyBase::atom_tuple_to_tuple_index)
        .def("tuple_index_to_atom_tuple", &NoveltyBase::tuple_index_to_atom_tuple)
        .def("get_max_tuple_size", &NoveltyBase::get_max_tuple_size)
        .def("get_num_atoms", &NoveltyBase::get_num_atoms)
        .def("get_num_tuples", &NoveltyBase::get_num_tuples)
    ;

    py::class_<TupleIndexGenerator>(m, "TupleIndexGenerator")
        .def(py::init<std::shared_ptr<const NoveltyBase>, const AtomIndices&>())
        .def("__copy__", [](const TupleIndexGenerator& generator, py::object){ return TupleIndexGenerator(generator); })
        .def("__deepcopy__", [](const TupleIndexGenerator& generator, py::object){ return TupleIndexGenerator(generator); })
        .def("__iter__", [](TupleIndexGenerator &generator) { return py::make_iterator(generator.begin(), generator.end()); },
                         py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
    ;

    py::class_<NoveltyTable>(m, "NoveltyTable")
        .def(py::init<int>())
        .def("__copy__", [](const NoveltyTable& table, py::object){ return NoveltyTable(table); })
        .def("__deepcopy__", [](const NoveltyTable& table, py::object){ return NoveltyTable(table); })
        .def("reset_novelty", &NoveltyTable::reset_novelty)
        .def("compute_novel_tuple_indices", [](NoveltyTable& self, TupleIndexGenerator generator){ return self.compute_novel_tuple_indices(std::move(generator));})
        .def("insert", [](NoveltyTable& self, TupleIndexGenerator generator, bool stop_if_novel){ return self.insert(std::move(generator), stop_if_novel);}, py::arg("tuple_index_generator"), py::arg("stop_if_novel") = true)
    ;

    py::class_<TupleNode>(m, "TupleNode")
        .def(py::init<TupleIndex, const StateIndices&>())
        .def(py::init<TupleIndex, StateIndices&&>())
        .def("__copy__", [](const TupleNode& node, py::object){ return TupleNode(node); })
        .def("__deepcopy__", [](const TupleNode& node, py::object){ return TupleNode(node); })
        .def("__str__", &TupleNode::str)
        .def("add_predecessor", &TupleNode::add_predecessor)
        .def("add_successor", &TupleNode::add_successor)
        .def("get_tuple_index", &TupleNode::get_tuple_index)
        .def("get_state_indices", &TupleNode::get_state_indices, py::return_value_policy::reference)
        .def("get_predecessors", &TupleNode::get_predecessors, py::return_value_policy::reference)
        .def("get_successors", &TupleNode::get_successors, py::return_value_policy::reference)
    ;

    py::class_<TupleGraph>(m, "TupleGraph")
        .def(py::init<std::shared_ptr<const NoveltyBase>, std::shared_ptr<const StateSpace>, StateIndex, int>())
        .def("__copy__", [](const TupleGraph& graph, py::object){ return TupleGraph(graph); })
        .def("__deepcopy__", [](const TupleGraph& graph, py::object){ return TupleGraph(graph); })
        .def("__str__", &TupleGraph::str)
        .def("to_dot", &TupleGraph::to_dot)
        .def("get_tuple_nodes_by_distance", &TupleGraph::get_tuple_nodes_by_distance, py::return_value_policy::reference)
        .def("get_state_indices_by_distance", &TupleGraph::get_state_indices_by_distance, py::return_value_policy::reference)
        .def("get_root_state_index", &TupleGraph::get_root_state_index)
        .def("get_width", &TupleGraph::get_width)
    ;
}
