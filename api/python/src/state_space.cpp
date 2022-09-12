#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors
#include <pybind11/iostream.h>
#include <pybind11/functional.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/state_space.h"


namespace py = pybind11;

using namespace dlplan::core;
using namespace dlplan::state_space;


void init_state_space(py::module_ &m) {
    py::class_<StateSpace>(m, "StateSpace")
        .def(py::init<std::shared_ptr<const InstanceInfo>, States, StateIndex, AdjacencyList, StateIndicesSet>())
        .def("prune_states", &StateSpace::prune_states)
        .def("compute_distances", &StateSpace::compute_distances)
        .def("for_each_state_index", &StateSpace::for_each_state_index)
        .def("for_each_forward_successor_state_index", &StateSpace::for_each_forward_successor_state_index)
        .def("is_goal", &StateSpace::is_goal)
        .def("is_nongoal", &StateSpace::is_nongoal)
        .def("is_deadend", &StateSpace::is_deadend)
        .def("is_alive", &StateSpace::is_alive)
        .def("is_solvable", &StateSpace::is_solvable)
        .def("is_trivially_solvable", &StateSpace::is_trivially_solvable)
        .def("get_states_ref", &StateSpace::get_states_ref)
        .def("get_state_ref", &StateSpace::get_state_ref)
        .def("get_num_states", &StateSpace::get_num_states)
        .def("get_initial_state_index", &StateSpace::get_initial_state_index)
        .def("get_forward_successor_state_indices", &StateSpace::get_forward_successor_state_indices)
        .def("get_backward_successor_state_indices", &StateSpace::get_backward_successor_state_indices)
        .def("get_goal_state_indices_ref", &StateSpace::get_goal_state_indices_ref)
        .def("get_deadend_state_indices_ref", &StateSpace::get_deadend_state_indices_ref)
        .def("get_goal_distances_ref", &StateSpace::get_goal_distances_ref)
        .def("get_instance_info", &StateSpace::get_instance_info)
        .def("print", &StateSpace::print)
    ;

    py::class_<StateSpaceGenerator>(m, "StateSpaceGenerator")
        .def(py::init<>())
        .def("generate_state_space", &StateSpaceGenerator::generate_state_space, py::arg("domain_file"), py::arg("instance_file"), py::arg("vocabulary_info") = nullptr)
    ;
}