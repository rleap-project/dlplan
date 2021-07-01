#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include <dlplan/generator.h>

namespace py = pybind11;

using namespace dlplan;

void init_generator(py::module_ &m) {
    py::class_<generator::FeatureGenerator, std::shared_ptr<generator::FeatureGenerator>>(m, "FeatureGenerator")
        .def(py::init<std::shared_ptr<core::SyntacticElementFactory>, int, int>())
        .def("generate", &generator::FeatureGenerator::generate)
    ;

    py::class_<generator::Numerical>(m, "NumericalFeature")
        .def(py::init<std::string, std::vector<int>>())
        .def("get_repr", &generator::Numerical::get_repr)
        .def("get_state_evaluations", &generator::Numerical::get_state_evaluations)
        .def(py::pickle(
            [](const generator::Numerical& n) {
                return py::make_tuple(n.get_repr(), n.get_state_evaluations());
            },
            [](py::tuple t) {
                return generator::Numerical(t[0].cast<std::string>(), t[1].cast<std::vector<int>>());
            }
        ))
    ;

    py::class_<generator::Boolean>(m, "BooleanFeature")
        .def(py::init<std::string, std::vector<bool>>())
        .def("get_repr", &generator::Boolean::get_repr)
        .def("get_state_evaluations", &generator::Boolean::get_state_evaluations)
        .def(py::pickle(
            [](const generator::Boolean& b) {
                return py::make_tuple(b.get_repr(), b.get_state_evaluations());
            },
            [](py::tuple t) {
                return generator::Boolean(t[0].cast<std::string>(), t[1].cast<std::vector<bool>>());
            }
        ))
    ;

    py::class_<generator::FeatureCollection>(m, "FeatureCollection")
        .def(py::init<>())
        .def("get_boolean_features", &generator::FeatureCollection::get_boolean_features)
        .def("get_numerical_features", &generator::FeatureCollection::get_numerical_features)
        .def("add_boolean_feature", [](generator::FeatureCollection& feature_collection, generator::Boolean b){ return feature_collection.generator::FeatureCollection::add_boolean_feature(std::move(b)); })
        .def("add_numerical_feature", [](generator::FeatureCollection& feature_collection, generator::Numerical n){ return feature_collection.generator::FeatureCollection::add_numerical_feature(std::move(n)); })
        .def(py::pickle(
            [](const generator::FeatureCollection& c){
                return py::make_tuple(c.get_boolean_features(), c.get_numerical_features());
            },
            [](py::tuple t) {
                generator::FeatureCollection c;
                for (generator::Boolean b : t[0].cast<std::vector<generator::Boolean>>()) {
                    c.add_boolean_feature(std::move(b));
                }
                for (generator::Numerical n : t[1].cast<std::vector<generator::Numerical>>()) {
                    c.add_numerical_feature(std::move(n));
                }
                return c;
            }
        ))
    ;
}
