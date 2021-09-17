#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include <dlplan/generator.h>

namespace py = pybind11;

using namespace dlplan;

void init_generator(py::module_ &m) {
    py::class_<generator::FeatureGenerator, std::shared_ptr<generator::FeatureGenerator>>(m, "FeatureGenerator")
        .def(py::init<std::shared_ptr<core::SyntacticElementFactory>, int, int, int, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool>(), py::arg("factory"), py::arg("complexity"), py::arg("time_limit"), py::arg("feature_limit"), py::arg("generate_empty_boolean") = true, py::arg("generate_all_concept") = true, py::arg("generate_and_concept") = true, py::arg("generate_bot_concept") = true, py::arg("generate_diff_concept") = true, py::arg("generate_equal_concept") = true, py::arg("generate_not_concept") = true, py::arg("generate_one_of_concept") = true, py::arg("generate_or_concept") = true, py::arg("generate_primitive_concept") = true, py::arg("generate_projection_concept") = true, py::arg("generate_some_concept") = true, py::arg("generate_subset_concept") = true, py::arg("generate_top_concept") = true, py::arg("generate_concept_distance_numerical") = true, py::arg("generate_count_numerical") = true, py::arg("generate_role_distance_numerical") = true, py::arg("generate_sum_concept_distance_numerical") = true, py::arg("generate_sum_role_distance_numerical") = true, py::arg("generate_and_role") = true, py::arg("generate_compose_role") = true, py::arg("generate_diff_role") = true, py::arg("generate_identity_role") = true, py::arg("generate_inverse_role") = true, py::arg("generate_not_role") = true, py::arg("generate_or_role") = true, py::arg("generate_primitive_role") = true, py::arg("generate_restrict_role") = true, py::arg("generate_top_role") = true, py::arg("generate_transitive_closure_role") = true, py::arg("generate_transitive_reflexive_closure_role") = true)
        .def("generate", &generator::FeatureGenerator::generate)
    ;
}
