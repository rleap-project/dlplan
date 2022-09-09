#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors
#include <pybind11/iostream.h>


#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/weisfeiler_lehman.h"


namespace py = pybind11;

using namespace dlplan;

void init_weisfeiler_lehman(py::module_ &m) {
    py::class_<weisfeiler_lehman::WeisfeilerLehman>(m, "WeisfeilerLehman")
        .def(py::init<>())
        .def("compute_colors_for_state_space", &weisfeiler_lehman::WeisfeilerLehman::compute_colors_for_state_space)
    ;
}
