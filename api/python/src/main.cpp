#include <pybind11/pybind11.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include <dlp/core.h>

namespace py = pybind11;

using namespace dlp::core;

PYBIND11_MODULE(_dlplan, m) {
    m.doc() = "Python bindings for the dlplan description logics first-order features for planning";

    py::class_<Predicate>(m, "Predicate")
        .def("name", &Predicate::get_name)
        .def("arity", &Predicate::get_arity)
        .def("index", &Predicate::get_predicate_idx)
    ;

    py::class_<VocabularyInfo>(m, "VocabularyInfo")
        .def(py::init<>())
        .def("add_predicate", &VocabularyInfo::add_predicate)
        .def("exists_predicate_name", &VocabularyInfo::exists_predicate_name)
    ;

    py::class_<InstanceInfo>(m, "InstanceInfo")
        .def(py::init<std::shared_ptr<VocabularyInfo>>())
        .def("add_atom", &InstanceInfo::add_atom)
        .def("add_static_atom", &InstanceInfo::add_static_atom)
//        .def("parse_state", &InstanceInfo::parse_state)
        // TODO Will need to disambiguate:
//        .def("convert_state", py::overload_cast<const std::vector<Atom> &>(&InstanceInfo::convert_state))
//        .def("convert_state", py::overload_cast<const std::vector<int> &>(&InstanceInfo::convert_state))
//        .def("convert_state", &InstanceInfo::convert_state)
    ;

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}