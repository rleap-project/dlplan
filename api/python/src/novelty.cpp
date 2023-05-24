#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/novelty.h"

namespace py = pybind11;

using namespace dlplan::novelty;
using namespace dlplan::state_space;


void init_novelty(py::module_ &m_novelty) {
    py::class_<NoveltyBase, std::shared_ptr<NoveltyBase>>(m_novelty, "NoveltyBase")
        .def(py::init<int, int>())
        .def("atom_tuple_to_tuple_index", &NoveltyBase::atom_tuple_to_tuple_index)
        .def("tuple_index_to_atom_tuple", &NoveltyBase::tuple_index_to_atom_tuple)
        .def("get_max_tuple_size", &NoveltyBase::get_max_tuple_size)
        .def("get_num_atoms", &NoveltyBase::get_num_atoms)
        .def("get_num_tuples", &NoveltyBase::get_num_tuples)
    ;

    py::class_<TupleIndexGenerator>(m_novelty, "TupleIndexGenerator")
        .def(py::init<std::shared_ptr<const NoveltyBase>, const AtomIndices&>())
        .def("__iter__", [](TupleIndexGenerator &generator) { return py::make_iterator(generator.begin(), generator.end()); },
                         py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
    ;

    py::class_<NoveltyTable>(m_novelty, "NoveltyTable")
        .def(py::init<int>())
        .def("reset_novelty", &NoveltyTable::reset_novelty)
        .def("compute_novel_tuple_indices", [](NoveltyTable& self, TupleIndexGenerator generator){ return self.compute_novel_tuple_indices(std::move(generator));})
        .def("insert", [](NoveltyTable& self, TupleIndexGenerator generator, bool stop_if_novel){ return self.insert(std::move(generator), stop_if_novel);}, py::arg("tuple_index_generator"), py::arg("stop_if_novel") = true)
        .def("insert", [](NoveltyTable& self, const TupleIndices& tuple_indices, bool stop_if_novel){ return self.insert(tuple_indices, stop_if_novel);}, py::arg("tuple_index_generator"), py::arg("stop_if_novel") = true)
    ;        

    py::class_<TupleNode>(m_novelty, "TupleNode")
        .def(py::init<TupleIndex, const StateIndices&>())
        .def(py::init<TupleIndex, StateIndices&&>())
        .def("__str__", &TupleNode::str)
        .def("get_tuple_index", &TupleNode::get_tuple_index)
        .def("get_state_indices", &TupleNode::get_state_indices, py::return_value_policy::reference)
        .def("get_predecessors", &TupleNode::get_predecessors, py::return_value_policy::reference)
        .def("get_successors", &TupleNode::get_successors, py::return_value_policy::reference)
    ;

    py::class_<TupleGraph>(m_novelty, "TupleGraph")
        .def(py::init<std::shared_ptr<const NoveltyBase>, std::shared_ptr<const StateSpace>, StateIndex, int>())
        .def("__str__", &TupleGraph::str)
        .def("to_dot", &TupleGraph::to_dot)
        .def("get_tuple_nodes_by_distance", &TupleGraph::get_tuple_nodes_by_distance, py::return_value_policy::reference)
        .def("get_state_indices_by_distance", &TupleGraph::get_state_indices_by_distance, py::return_value_policy::reference)
        .def("get_root_state_index", &TupleGraph::get_root_state_index)
        .def("get_width", &TupleGraph::get_width)
    ;
}
