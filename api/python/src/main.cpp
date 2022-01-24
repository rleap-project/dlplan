#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

void init_core(py::module_ &);
void init_generator(py::module_ &);
void init_policy(py::module_ &);

PYBIND11_MODULE(_dlplan, m) {
    m.doc() = "Python bindings for the dlplan description logics first-order features for planning";

    init_core(m);
    init_generator(m);
    init_policy(m);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
