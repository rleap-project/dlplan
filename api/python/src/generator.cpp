#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include <dlp/generator.h>

namespace py = pybind11;

using namespace dlp;

void init_generator(py::module_ &m) {
    py::class_<generator::FeatureGenerator>(m, "FeatureGenerator")
        .def(py::init<std::shared_ptr<core::SyntacticElementFactory>, int, int>())
        .def("generate", &generator::FeatureGenerator::generate)
    ;

    py::class_<generator::Numerical>(m, "NumericalFeature")
        .def("get_repr", &generator::Numerical::get_repr)
        .def("get_state_evaluations", &generator::Numerical::get_state_evaluations)
    ;

    py::class_<generator::Boolean>(m, "BooleanFeature")
        .def("get_repr", &generator::Boolean::get_repr)
        .def("get_state_evaluations", &generator::Boolean::get_state_evaluations)
    ;

    py::class_<generator::FeatureCollection>(m, "FeatureCollection")
        .def("get_boolean_features", &generator::FeatureCollection::get_boolean_features)
        .def("get_numerical_features", &generator::FeatureCollection::get_numerical_features)
    ;
}
