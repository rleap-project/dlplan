#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/generator.h"

namespace py = pybind11;

using namespace dlplan;

void init_generator(py::module_ &m) {
    py::class_<generator::FeatureGenerator, std::shared_ptr<generator::FeatureGenerator>>(m, "FeatureGenerator")
        .def(py::init<>())
        .def("generate", &generator::FeatureGenerator::generate)
        .def("set_generate_empty_boolean", &generator::FeatureGenerator::set_generate_empty_boolean)
        .def("set_generate_inclusion_boolean", &generator::FeatureGenerator::set_generate_inclusion_boolean)
        .def("set_generate_nullary_boolean", &generator::FeatureGenerator::set_generate_nullary_boolean)
        .def("set_generate_all_concept", &generator::FeatureGenerator::set_generate_all_concept)
        .def("set_generate_and_concept", &generator::FeatureGenerator::set_generate_and_concept)
        .def("set_generate_bot_concept", &generator::FeatureGenerator::set_generate_bot_concept)
        .def("set_generate_diff_concept", &generator::FeatureGenerator::set_generate_diff_concept)
        .def("set_generate_equal_concept", &generator::FeatureGenerator::set_generate_equal_concept)
        .def("set_generate_not_concept", &generator::FeatureGenerator::set_generate_not_concept)
        .def("set_generate_one_of_concept", &generator::FeatureGenerator::set_generate_one_of_concept)
        .def("set_generate_or_concept", &generator::FeatureGenerator::set_generate_or_concept)
        .def("set_generate_primitive_concept", &generator::FeatureGenerator::set_generate_primitive_concept)
        .def("set_generate_projection_concept", &generator::FeatureGenerator::set_generate_projection_concept)
        .def("set_generate_some_concept", &generator::FeatureGenerator::set_generate_some_concept)
        .def("set_generate_subset_concept", &generator::FeatureGenerator::set_generate_subset_concept)
        .def("set_generate_top_concept", &generator::FeatureGenerator::set_generate_top_concept)
        .def("set_generate_concept_distance_numerical", &generator::FeatureGenerator::set_generate_concept_distance_numerical)
        .def("set_generate_count_numerical", &generator::FeatureGenerator::set_generate_count_numerical)
        .def("set_generate_role_distance_numerical", &generator::FeatureGenerator::set_generate_role_distance_numerical)
        .def("set_generate_sum_concept_distance_numerical", &generator::FeatureGenerator::set_generate_sum_concept_distance_numerical)
        .def("set_generate_sum_role_distance_numerical", &generator::FeatureGenerator::set_generate_sum_role_distance_numerical)
        .def("set_generate_and_role", &generator::FeatureGenerator::set_generate_and_role)
        .def("set_generate_compose_role", &generator::FeatureGenerator::set_generate_compose_role)
        .def("set_generate_diff_role", &generator::FeatureGenerator::set_generate_diff_role)
        .def("set_generate_identity_role", &generator::FeatureGenerator::set_generate_identity_role)
        .def("set_generate_inverse_role", &generator::FeatureGenerator::set_generate_inverse_role)
        .def("set_generate_not_role", &generator::FeatureGenerator::set_generate_not_role)
        .def("set_generate_or_role", &generator::FeatureGenerator::set_generate_or_role)
        .def("set_generate_primitive_role", &generator::FeatureGenerator::set_generate_primitive_role)
        .def("set_generate_restrict_role", &generator::FeatureGenerator::set_generate_restrict_role)
        .def("set_generate_top_role", &generator::FeatureGenerator::set_generate_top_role)
        .def("set_generate_transitive_closure_role", &generator::FeatureGenerator::set_generate_transitive_closure_role)
        .def("set_generate_transitive_reflexive_closure_role", &generator::FeatureGenerator::set_generate_transitive_reflexive_closure_role)
    ;
}
