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


void init_policy(py::module_ &m_policy) {
    py::class_<BaseCondition, std::shared_ptr<BaseCondition>>(m_policy, "BaseCondition")
        .def("__repr__", &BaseCondition::compute_repr)
        .def("__str__", &BaseCondition::str)
        .def("evaluate", py::overload_cast<const State&>(&BaseCondition::evaluate, py::const_))
        .def("evaluate",  py::overload_cast<const State&, DenotationsCaches&>(&BaseCondition::evaluate, py::const_))
        .def("get_index", &BaseCondition::get_index)
        .def("get_boolean", &BaseCondition::get_boolean)
        .def("get_numerical", &BaseCondition::get_numerical)
    ;

    py::class_<BaseEffect, std::shared_ptr<BaseEffect>>(m_policy, "BaseEffect")
        .def("__repr__", &BaseEffect::compute_repr)
        .def("__str__", &BaseEffect::str)
        .def("evaluate", py::overload_cast<const State&, const State&>(&BaseEffect::evaluate, py::const_))
        .def("evaluate",  py::overload_cast<const State&, const State&, DenotationsCaches&>(&BaseEffect::evaluate, py::const_))
        .def("get_index", &BaseEffect::get_index)
        .def("get_boolean", &BaseEffect::get_boolean)
        .def("get_numerical", &BaseEffect::get_numerical)
    ;

    py::class_<Rule, std::shared_ptr<Rule>>(m_policy, "Rule")
        .def("__repr__", &Rule::compute_repr)
        .def("__str__", &Rule::str)
        .def("evaluate_conditions", py::overload_cast<const State&>(&Rule::evaluate_conditions, py::const_))
        .def("evaluate_conditions", py::overload_cast<const State&, DenotationsCaches&>(&Rule::evaluate_conditions, py::const_))
        .def("evaluate_effects", py::overload_cast<const State&, const State&>(&Rule::evaluate_effects, py::const_))
        .def("evaluate_effects", py::overload_cast<const State&, const State&, DenotationsCaches&>(&Rule::evaluate_effects, py::const_))
        .def("get_index", &Rule::get_index)
        .def("get_conditions", &Rule::get_conditions, py::return_value_policy::reference)
        .def("get_effects", &Rule::get_effects, py::return_value_policy::reference)
    ;

    py::class_<Policy, std::shared_ptr<Policy>>(m_policy, "Policy")
        .def("__repr__", &Policy::compute_repr)
        .def("__str__", &Policy::str)
        .def("evaluate", py::overload_cast<const State&, const State&>(&Policy::evaluate, py::const_))
        .def("evaluate", py::overload_cast<const State&, const State&, DenotationsCaches&>(&Policy::evaluate, py::const_))
        .def("evaluate_conditions", py::overload_cast<const State&>(&Policy::evaluate_conditions, py::const_))
        .def("evaluate_conditions", py::overload_cast<const State&, DenotationsCaches&>(&Policy::evaluate_conditions, py::const_))
        .def("evaluate_effects", py::overload_cast<const State&, const State&, const std::vector<std::shared_ptr<const Rule>>&>(&Policy::evaluate_effects, py::const_))
        .def("evaluate_effects", py::overload_cast<const State&, const State&, const std::vector<std::shared_ptr<const Rule>>&, DenotationsCaches&>(&Policy::evaluate_effects, py::const_))
        .def("get_rules", &Policy::get_rules, py::return_value_policy::reference)
        .def("get_booleans", &Policy::get_booleans, py::return_value_policy::reference)
        .def("get_numericals", &Policy::get_numericals, py::return_value_policy::reference)
    ;

    py::class_<PolicyFactory, std::shared_ptr<PolicyFactory>>(m_policy, "PolicyFactory")
        .def(py::init<>())
        .def("add_pos_condition", &PolicyFactory::add_pos_condition)
        .def("add_neg_condition", &PolicyFactory::add_neg_condition)
        .def("add_gt_condition", &PolicyFactory::add_gt_condition)
        .def("add_eq_condition", &PolicyFactory::add_eq_condition)
        .def("add_pos_effect", &PolicyFactory::add_pos_effect)
        .def("add_neg_effect", &PolicyFactory::add_neg_effect)
        .def("add_inc_effect", &PolicyFactory::add_inc_effect)
        .def("add_dec_effect", &PolicyFactory::add_dec_effect)
        .def("add_bot_effect", py::overload_cast<const std::shared_ptr<const Boolean>&>(&PolicyFactory::add_bot_effect))
        .def("add_bot_effect", py::overload_cast<const std::shared_ptr<const Numerical>&>(&PolicyFactory::add_bot_effect))
        .def("add_rule", &PolicyFactory::add_rule)
        .def("add_policy", &PolicyFactory::add_policy)
    ;

    py::class_<PolicyMinimizer>(m_policy, "PolicyMinimizer")
        .def(py::init<>())
        .def("minimize", py::overload_cast<const std::shared_ptr<const Policy>&, PolicyFactory&>(&PolicyMinimizer::minimize, py::const_))
        .def("minimize", py::overload_cast<const std::shared_ptr<const Policy>&, const StatePairs&, const StatePairs&, PolicyFactory&>(&PolicyMinimizer::minimize, py::const_))
    ;

    py::class_<PolicyReader>(m_policy, "PolicyReader")
        .def(py::init<>())
        .def("read", &PolicyReader::read)
    ;

    py::class_<PolicyWriter>(m_policy, "PolicyWriter")
        .def(py::init<>())
        .def("write", &PolicyWriter::write)
    ;
}