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
    py::class_<policy::BaseCondition, std::shared_ptr<policy::BaseCondition>>(m, "BaseCondition")
        .def("evaluate", py::overload_cast<const core::State&>(&policy::BaseCondition::evaluate, py::const_))
        .def("evaluate",  py::overload_cast<const core::State&, evaluator::EvaluationCache&>(&policy::BaseCondition::evaluate, py::const_))
        .def("get_base_feature", &policy::BaseCondition::get_base_feature)
        .def("compute_repr", &policy::BaseCondition::compute_repr)
        .def("str", &policy::BaseCondition::str)
    ;

    py::class_<policy::BaseEffect, std::shared_ptr<policy::BaseEffect>>(m, "BaseEffect")
        .def("evaluate", py::overload_cast<const core::State&, const core::State&>(&policy::BaseEffect::evaluate, py::const_))
        .def("evaluate",  py::overload_cast<const core::State&, const core::State&, evaluator::EvaluationCache&>(&policy::BaseEffect::evaluate, py::const_))
        .def("get_base_feature", &policy::BaseEffect::get_base_feature)
        .def("compute_repr", &policy::BaseEffect::compute_repr)
        .def("str", &policy::BaseEffect::str)
    ;

    py::class_<policy::Rule, std::shared_ptr<policy::Rule>>(m, "Rule")
        .def("evaluate_conditions", py::overload_cast<const core::State&>(&policy::Rule::evaluate_conditions, py::const_))
        .def("evaluate_conditions", py::overload_cast<const core::State&, evaluator::EvaluationCache&>(&policy::Rule::evaluate_conditions, py::const_))
        .def("evaluate_effects", py::overload_cast<const core::State&, const core::State&>(&policy::Rule::evaluate_effects, py::const_))
        .def("evaluate_effects", py::overload_cast<const core::State&, const core::State&, evaluator::EvaluationCache&>(&policy::Rule::evaluate_effects, py::const_))
        .def("get_conditions", &policy::Rule::get_conditions)
        .def("get_effects", &policy::Rule::get_effects)
        .def("compute_repr", &policy::Rule::compute_repr)
        .def("str", &policy::Rule::str)
    ;

    py::class_<policy::Policy>(m, "Policy")
        .def("evaluate_lazy", py::overload_cast<const core::State&, const core::State&>(&policy::Policy::evaluate_lazy, py::const_))
        .def("evaluate_lazy", py::overload_cast<const core::State&, const core::State&, evaluator::EvaluationCache&>(&policy::Policy::evaluate_lazy, py::const_))
        .def("evaluate_conditions_eager", py::overload_cast<const core::State&>(&policy::Policy::evaluate_conditions_eager, py::const_))
        .def("evaluate_conditions_eager", py::overload_cast<const core::State&, evaluator::EvaluationCache&>(&policy::Policy::evaluate_conditions_eager, py::const_))
        .def("evaluate_effects_lazy", py::overload_cast<const core::State&, const core::State&, const std::vector<std::shared_ptr<const policy::Rule>>&>(&policy::Policy::evaluate_effects_lazy, py::const_))
        .def("evaluate_effects_lazy", py::overload_cast<const core::State&, const core::State&, const std::vector<std::shared_ptr<const policy::Rule>>&, evaluator::EvaluationCache&>(&policy::Policy::evaluate_effects_lazy, py::const_))
        .def("get_rules", &policy::Policy::get_rules)
        .def("get_boolean_features", &policy::Policy::get_boolean_features)
        .def("get_numerical_features", &policy::Policy::get_numerical_features)
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
        .def("add_bot_effect", py::overload_cast<std::shared_ptr<const core::Boolean>>(&policy::PolicyBuilder::add_bot_effect))
        .def("add_bot_effect", py::overload_cast<std::shared_ptr<const core::Numerical>>(&policy::PolicyBuilder::add_bot_effect))
        .def("add_rule", &policy::PolicyBuilder::add_rule)
        .def("get_result", &policy::PolicyBuilder::get_result)
    ;

    py::class_<policy::PolicyMinimizer>(m, "PolicyMinimizer")
        .def(py::init<>())
        .def("minimize", py::overload_cast<const policy::Policy&>(&policy::PolicyMinimizer::minimize, py::const_))
        .def("minimize", py::overload_cast<const policy::Policy&, const core::StatePairs&, const core::StatePairs&>(&policy::PolicyMinimizer::minimize, py::const_))
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