#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors
#include <pybind11/iostream.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/policy.h"
#include "../../../include/dlplan/core.h"

#include "../../../src/policy/evaluation_cache.h"


namespace py = pybind11;

using namespace dlplan;

void init_policy(py::module_ &m) {
        py::class_<policy::PolicyRoot, std::shared_ptr<policy::PolicyRoot>>(m, "PolicyRoot")
    ;

    py::class_<policy::EvaluationCaches>(m, "EvaluationCaches")
    ;

    py::class_<policy::State>(m, "PolicyState")
        .def(py::init<int, dlplan::core::State>())
        .def("get_index", &policy::State::get_index)
        .def("get_state", &policy::State::get_state)
    ;

    py::class_<policy::BooleanFeature, std::shared_ptr<policy::BooleanFeature>>(m, "BooleanFeature")
        .def("get_index", &policy::BooleanFeature::get_index)
        .def("get_root", &policy::BooleanFeature::get_root)
        .def("BooleanFeature", &policy::BooleanFeature::compute_repr)
    ;

    py::class_<policy::NumericalFeature, std::shared_ptr<policy::NumericalFeature>>(m, "NumericalFeature")
        .def("get_index", &policy::NumericalFeature::get_index)
        .def("get_root", &policy::NumericalFeature::get_root)
        .def("BooleanFeature", &policy::NumericalFeature::compute_repr)
    ;

    py::class_<policy::BaseCondition, std::shared_ptr<policy::BaseCondition>>(m, "BaseCondition")
        .def("evaluate", &policy::BaseCondition::evaluate)
        .def("clone", &policy::BaseCondition::clone)
        //.def("get_root", &policy::BaseCondition::get_root)
        .def("compute_repr", &policy::BaseCondition::compute_repr)
    ;

    py::class_<policy::BaseEffect, std::shared_ptr<policy::BaseEffect>>(m, "BaseEffect")
        .def("evaluate", &policy::BaseEffect::evaluate)
        .def("clone", &policy::BaseEffect::clone)
        //.def("get_root", &policy::BaseEffect::get_root)
        .def("compute_repr", &policy::BaseEffect::compute_repr)
    ;

    py::class_<policy::Rule, std::shared_ptr<policy::Rule>>(m, "Rule")
        .def("evaluate_conditions", &policy::Rule::evaluate_conditions)
        .def("evaluate_effects", &policy::Rule::evaluate_effects)
        .def("get_root", &policy::Rule::get_root)
        .def("compute_repr", &policy::Rule::compute_repr)
    ;

    py::class_<policy::Policy>(m, "Policy")
        .def("evaluate", &policy::Policy::evaluate)
        .def("get_boolean_features", &policy::Policy::get_boolean_features)
        .def("get_numerical_features", &policy::Policy::get_numerical_features)
        .def("get_root", &policy::Policy::get_root)
        .def("compute_repr", &policy::Policy::compute_repr)
    ;

    py::class_<policy::PolicyBuilder>(m, "PolicyBuilder")
        .def(py::init<>())
        .def("add_boolean_feature", &policy::PolicyBuilder::add_boolean_feature)
        .def("add_numerical_feature", &policy::PolicyBuilder::add_numerical_feature)
        .def("add_b_pos_condition", &policy::PolicyBuilder::add_b_pos_condition)
        .def("add_b_neg_condition", &policy::PolicyBuilder::add_b_neg_condition)
        .def("add_n_gt_condition", &policy::PolicyBuilder::add_n_gt_condition)
        .def("add_n_eq_condition", &policy::PolicyBuilder::add_n_eq_condition)
        .def("add_b_pos_effect", &policy::PolicyBuilder::add_b_pos_effect)
        .def("add_b_neg_effect", &policy::PolicyBuilder::add_b_neg_effect)
        .def("add_b_bot_effect", &policy::PolicyBuilder::add_b_bot_effect)
        .def("add_n_inc_effect", &policy::PolicyBuilder::add_n_inc_effect)
        .def("add_n_dec_effect", &policy::PolicyBuilder::add_n_dec_effect)
        .def("add_n_bot_effect", &policy::PolicyBuilder::add_n_bot_effect)
        .def("add_rule", &policy::PolicyBuilder::add_rule)
        .def("get_result", &policy::PolicyBuilder::get_result)
    ;

    py::class_<policy::PolicyReader>(m, "PolicyReader")
        .def(py::init<>())
        .def("read", &policy::PolicyReader::read)
    ;

    py::class_<policy::PolicyWriter>(m, "PolicyWriter")
        .def(py::init<>())
        .def("write", &policy::PolicyWriter::write)
    ;
}