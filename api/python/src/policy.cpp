#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors
#include <pybind11/iostream.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/policy.h"
#include "../../../include/dlplan/core.h"


namespace py = pybind11;

using namespace dlplan;

void init_policy(py::module_ &m) {
        py::class_<policy::PolicyRoot, std::shared_ptr<policy::PolicyRoot>>(m, "PolicyRoot")
    ;

    py::class_<policy::BaseFeature, std::shared_ptr<policy::BaseFeature>>(m, "BaseFeature")
        .def("get_index", &policy::BaseFeature::get_index)
        .def("get_root", &policy::BaseFeature::get_root)
        .def("compute_repr", &policy::BaseFeature::compute_repr)
        .def("str", &policy::BaseFeature::str)
    ;

    py::class_<policy::BooleanFeature, std::shared_ptr<policy::BooleanFeature>>(m, "BooleanFeature")
        .def("evaluate", &policy::BooleanFeature::evaluate)
        .def("get_index", &policy::BooleanFeature::get_index)
        .def("get_root", &policy::BooleanFeature::get_root)
        .def("compute_repr", &policy::BooleanFeature::compute_repr)
        .def("str", &policy::BooleanFeature::str)
    ;

    py::class_<policy::NumericalFeature, std::shared_ptr<policy::NumericalFeature>>(m, "NumericalFeature")
        .def("evaluate", &policy::NumericalFeature::evaluate)
        .def("get_index", &policy::NumericalFeature::get_index)
        .def("get_root", &policy::NumericalFeature::get_root)
        .def("compute_repr", &policy::NumericalFeature::compute_repr)
        .def("str", &policy::NumericalFeature::str)
    ;

    py::class_<policy::BaseCondition, std::shared_ptr<policy::BaseCondition>>(m, "BaseCondition")
        .def("evaluate", &policy::BaseCondition::evaluate)
        .def("get_root", &policy::BaseCondition::get_root)
        .def("get_base_feature", &policy::BaseCondition::get_base_feature)
        .def("compute_repr", &policy::BaseCondition::compute_repr)
        .def("str", &policy::BaseCondition::str)
    ;

    py::class_<policy::BaseEffect, std::shared_ptr<policy::BaseEffect>>(m, "BaseEffect")
        .def("evaluate", &policy::BaseEffect::evaluate)
        .def("get_root", &policy::BaseEffect::get_root)
        .def("get_base_feature", &policy::BaseEffect::get_base_feature)
        .def("compute_repr", &policy::BaseEffect::compute_repr)
        .def("str", &policy::BaseEffect::str)
    ;

    py::class_<policy::Rule, std::shared_ptr<policy::Rule>>(m, "Rule")
        .def("evaluate_conditions", &policy::Rule::evaluate_conditions)
        .def("evaluate_effects", &policy::Rule::evaluate_effects)
        .def("get_root", &policy::Rule::get_root)
        .def("get_conditions", &policy::Rule::get_conditions)
        .def("get_effects", &policy::Rule::get_effects)
        .def("compute_repr", &policy::Rule::compute_repr)
        .def("str", &policy::Rule::str)
    ;

    py::class_<policy::Policy>(m, "Policy")
        .def("evaluate_lazy", &policy::Policy::evaluate_lazy)
        .def("evaluate_conditions_eager", &policy::Policy::evaluate_conditions_eager)
        .def("evaluate_effects_lazy", &policy::Policy::evaluate_effects_lazy)
        .def("get_rules", &policy::Policy::get_rules)
        .def("get_boolean_features", &policy::Policy::get_boolean_features)
        .def("get_numerical_features", &policy::Policy::get_numerical_features)
        .def("get_root", &policy::Policy::get_root)
        .def("compute_repr", &policy::Policy::compute_repr)
        .def("str", &policy::Policy::str)
    ;

    py::class_<policy::PolicyBuilder>(m, "PolicyBuilder")
        .def(py::init<>())
        .def("add_boolean_feature", &policy::PolicyBuilder::add_boolean_feature)
        .def("add_numerical_feature", &policy::PolicyBuilder::add_numerical_feature)
        .def("add_pos_condition", &policy::PolicyBuilder::add_pos_condition)
        .def("add_neg_condition", &policy::PolicyBuilder::add_neg_condition)
        .def("add_gt_condition", &policy::PolicyBuilder::add_gt_condition)
        .def("add_eq_condition", &policy::PolicyBuilder::add_eq_condition)
        .def("add_pos_effect", &policy::PolicyBuilder::add_pos_effect)
        .def("add_neg_effect", &policy::PolicyBuilder::add_neg_effect)
        .def("add_inc_effect", &policy::PolicyBuilder::add_inc_effect)
        .def("add_dec_effect", &policy::PolicyBuilder::add_dec_effect)
        .def("add_bot_effect", py::overload_cast<std::shared_ptr<const policy::BooleanFeature>>(&policy::PolicyBuilder::add_bot_effect))
        .def("add_bot_effect", py::overload_cast<std::shared_ptr<const policy::NumericalFeature>>(&policy::PolicyBuilder::add_bot_effect))
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