#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors
#include <pybind11/iostream.h>
#include <pybind11/functional.h>
#include <pybind11/embed.h> // everything needed for embedding

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/state_space.h"


namespace py = pybind11;

using namespace dlplan::core;
using namespace dlplan::state_space;


void init_state_space(py::module_ &m_state_space) {
    py::class_<StateSpace, std::shared_ptr<StateSpace>>(m_state_space, "StateSpace")
        .def(py::init<std::shared_ptr<InstanceInfo>, StateMapping, StateIndex, AdjacencyList, StateIndicesSet>())
        .def(py::init<const StateSpace&, const StateIndicesSet&>())
        .def("__str__", &StateSpace::str)
        .def("compute_distances", &StateSpace::compute_distances)
        .def("compute_goal_distances", &StateSpace::compute_goal_distances)
        .def("is_goal", &StateSpace::is_goal)
        .def("to_dot", &StateSpace::to_dot)
        .def("set_initial_state_index", &StateSpace::set_initial_state_index)
        .def("set_goal_state_indices", &StateSpace::set_goal_state_indices)
        .def("get_states", &StateSpace::get_states)
        .def("get_initial_state_index", &StateSpace::get_initial_state_index)
        .def("get_forward_successor_state_indices", &StateSpace::get_forward_successor_state_indices)
        .def("get_backward_successor_state_indices", &StateSpace::get_backward_successor_state_indices)
        .def("get_goal_state_indices", &StateSpace::get_goal_state_indices)
        .def("get_instance_info", &StateSpace::get_instance_info)
    ;

    py::enum_<GeneratorExitCode>(m_state_space, "GeneratorExitCode")
        .value("COMPLETE", GeneratorExitCode::COMPLETE)
        .value("INCOMPLETE", GeneratorExitCode::INCOMPLETE)
        .value("FAIL", GeneratorExitCode::FAIL);

    py::class_<GeneratorResult>(m_state_space, "GeneratorResult")
        .def_readwrite("exit_code", &GeneratorResult::exit_code)
        .def_readwrite("state_space", &GeneratorResult::state_space)
    ;

    m_state_space.def("generate_state_space", &generate_state_space, py::arg("domain_file"), py::arg("instance_file"), py::arg("vocabulary_info") = nullptr, py::arg("index") = -1, py::arg("max_time") = std::numeric_limits<int>::max()-1, py::arg("max_num_states") = std::numeric_limits<int>::max()-1)
    ;
}
