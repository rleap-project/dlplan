#include "pybind11/pybind11.h"
#include "pybind11/stl.h"  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

void init_core(py::module_ &);
void init_generator(py::module_ &);
void init_novelty(py::module_ &);
void init_policy(py::module_ &);
void init_state_space(py::module_ &);

PYBIND11_MODULE(_dlplan, m) {
    m.doc() = "Python bindings for the dlplan description logics first-order features for planning";

    py::module_ m_core = m.def_submodule("core", "The core submodule.");
    py::module_ m_generator = m.def_submodule("generator", "The generator submodule.");
    py::module_ m_novelty = m.def_submodule("novelty", "The novelty submodule.");
    py::module_ m_policy = m.def_submodule("policy", "The policy submodule.");
    py::module_ m_state_space = m.def_submodule("state_space", "The state_space submodule.");

    init_core(m);
    init_generator(m);
    init_novelty(m);
    init_policy(m);
    init_state_space(m);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
