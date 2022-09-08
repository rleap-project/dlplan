#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors
#include <pybind11/iostream.h>
#include <pybind11/functional.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/state_space.h"


namespace py = pybind11;

using namespace dlplan;

void init_state_space(py::module_ &m) {
    py::class_<state_space::StateSpace>(m, "StateSpace")
        .def("for_each_state_index", &state_space::StateSpace::for_each_state_index)
        .def("for_each_forward_successor_state_index", &state_space::StateSpace::for_each_forward_successor_state_index)
    ;
}