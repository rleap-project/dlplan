#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors
#include <pybind11/iostream.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/core.h"
#include "../../../include/dlplan/policy.h"


namespace py = pybind11;

using namespace dlplan::core;
using namespace dlplan::policy;


void init_policy(py::module_ &m) {
    py::class_<BaseCondition, std::shared_ptr<BaseCondition>>(m, "BaseCondition")
        .def("__repr__", &BaseCondition::compute_repr)
        .def("__str__", &BaseCondition::str)
        .def("evaluate", py::overload_cast<const State&>(&BaseCondition::evaluate, py::const_))
        .def("evaluate",  py::overload_cast<const State&, DenotationsCaches&>(&BaseCondition::evaluate, py::const_))
        .def("get_index", &BaseCondition::get_index)
        .def("compute_repr", &BaseCondition::compute_repr)
        .def("str", &BaseCondition::str)
        .def("copy_to_builder", &BaseCondition::copy_to_builder)
    ;

    py::class_<BaseEffect, std::shared_ptr<BaseEffect>>(m, "BaseEffect")
        .def("__repr__", &BaseEffect::compute_repr)
        .def("__str__", &BaseEffect::str)
        .def("evaluate", py::overload_cast<const State&, const State&>(&BaseEffect::evaluate, py::const_))
        .def("evaluate",  py::overload_cast<const State&, const State&, DenotationsCaches&>(&BaseEffect::evaluate, py::const_))
        .def("get_index", &BaseEffect::get_index)
        .def("compute_repr", &BaseEffect::compute_repr)
        .def("str", &BaseEffect::str)
        .def("copy_to_builder", &BaseEffect::copy_to_builder)
    ;

    py::class_<Rule, std::shared_ptr<Rule>>(m, "Rule")
        .def("__repr__", &Rule::compute_repr)
        .def("__str__", &Rule::str)
        .def("evaluate_conditions", py::overload_cast<const State&>(&Rule::evaluate_conditions, py::const_))
        .def("evaluate_conditions", py::overload_cast<const State&, DenotationsCaches&>(&Rule::evaluate_conditions, py::const_))
        .def("evaluate_effects", py::overload_cast<const State&, const State&>(&Rule::evaluate_effects, py::const_))
        .def("evaluate_effects", py::overload_cast<const State&, const State&, DenotationsCaches&>(&Rule::evaluate_effects, py::const_))
        .def("get_index", &Rule::get_index)
        .def("get_conditions", &Rule::get_conditions)
        .def("get_effects", &Rule::get_effects)
        .def("compute_repr", &Rule::compute_repr)
        .def("str", &Rule::str)
        .def("copy_to_builder", &Rule::copy_to_builder)
    ;

    py::class_<Policy>(m, "Policy")
        .def("__repr__", &Policy::compute_repr)
        .def("__str__", &Policy::str)
        .def("evaluate_lazy", py::overload_cast<const State&, const State&>(&Policy::evaluate_lazy, py::const_))
        .def("evaluate_lazy", py::overload_cast<const State&, const State&, DenotationsCaches&>(&Policy::evaluate_lazy, py::const_))
        .def("evaluate_conditions_eager", py::overload_cast<const State&>(&Policy::evaluate_conditions_eager, py::const_))
        .def("evaluate_conditions_eager", py::overload_cast<const State&, DenotationsCaches&>(&Policy::evaluate_conditions_eager, py::const_))
        .def("evaluate_effects_lazy", py::overload_cast<const State&, const State&, const std::vector<std::shared_ptr<const Rule>>&>(&Policy::evaluate_effects_lazy, py::const_))
        .def("evaluate_effects_lazy", py::overload_cast<const State&, const State&, const std::vector<std::shared_ptr<const Rule>>&, DenotationsCaches&>(&Policy::evaluate_effects_lazy, py::const_))
        .def("get_rules", &Policy::get_rules, py::return_value_policy::reference)
        .def("compute_repr", &Policy::compute_repr)
        .def("str", &Policy::str)
        .def("copy_to_builder", &Policy::copy_to_builder)
    ;

    py::class_<PolicyBuilder>(m, "PolicyBuilder")
        .def(py::init<>())
        .def("add_boolean_feature", &PolicyBuilder::add_boolean_feature)
        .def("add_numerical_feature", &PolicyBuilder::add_numerical_feature)
        .def("add_pos_condition", &PolicyBuilder::add_pos_condition)
        .def("add_neg_condition", &PolicyBuilder::add_neg_condition)
        .def("add_gt_condition", &PolicyBuilder::add_gt_condition)
        .def("add_eq_condition", &PolicyBuilder::add_eq_condition)
        .def("add_pos_effect", &PolicyBuilder::add_pos_effect)
        .def("add_neg_effect", &PolicyBuilder::add_neg_effect)
        .def("add_inc_effect", &PolicyBuilder::add_inc_effect)
        .def("add_dec_effect", &PolicyBuilder::add_dec_effect)
        .def("add_bot_effect", py::overload_cast<std::shared_ptr<const Boolean>>(&PolicyBuilder::add_bot_effect))
        .def("add_bot_effect", py::overload_cast<std::shared_ptr<const Numerical>>(&PolicyBuilder::add_bot_effect))
        .def("add_rule", &PolicyBuilder::add_rule)
        .def("get_result", &PolicyBuilder::get_result)
        .def("get_booleans", &PolicyBuilder::get_booleans)
        .def("get_numericals", &PolicyBuilder::get_numericals)
    ;

    py::class_<PolicyMinimizer>(m, "PolicyMinimizer")
        .def(py::init<>())
        .def("minimize", py::overload_cast<const Policy&>(&PolicyMinimizer::minimize, py::const_))
        .def("minimize", py::overload_cast<const Policy&, const StatePairs&, const StatePairs&>(&PolicyMinimizer::minimize, py::const_))
    ;

    py::class_<PolicyReader>(m, "PolicyReader")
        .def(py::init<>())
        .def("read", &PolicyReader::read)
    ;

    py::class_<PolicyWriter>(m, "PolicyWriter")
        .def(py::init<>())
        .def("write", &PolicyWriter::write)
    ;
}