#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors
#include <pybind11/iostream.h>


#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/weisfeiler_lehman.h"


namespace py = pybind11;

using namespace dlplan::weisfeiler_lehman;


void init_weisfeiler_lehman(py::module_ &m) {
    py::class_<WeisfeilerLehman>(m, "WeisfeilerLehman")
        .def(py::init<>())
        .def("__copy__", [](const WeisfeilerLehman& wl, py::object){ return WeisfeilerLehman(wl); })
        .def("__deepcopy__", [](const WeisfeilerLehman& wl, py::object){ return WeisfeilerLehman(wl); })
        .def("compute_colors_for_state_space", &WeisfeilerLehman::compute_colors_for_state_space)
    ;
}
