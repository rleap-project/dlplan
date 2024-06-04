#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors
#include <pybind11/iostream.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/core.h"
#include "../../../include/dlplan/policy.h"
#include "../../../include/dlplan/policy/condition.h"
#include "../../../include/dlplan/policy/effect.h"

namespace py = pybind11;

using namespace dlplan;


void init_policy(py::module_ &m_policy) {
    py::class_<policy::NamedBoolean, std::shared_ptr<policy::NamedBoolean>>(m_policy, "NamedBoolean")
        .def("__str__", py::overload_cast<>(&policy::NamedBoolean::str, py::const_))
        .def("get_key", &policy::NamedBoolean::get_key)
        .def("get_element", &policy::NamedBoolean::get_element)
    ;

    py::class_<policy::NamedNumerical, std::shared_ptr<policy::NamedNumerical>>(m_policy, "NamedNumerical")
        .def("__str__", py::overload_cast<>(&policy::NamedNumerical::str, py::const_))
        .def("get_key", &policy::NamedNumerical::get_key)
        .def("get_element", &policy::NamedNumerical::get_element)
    ;

    py::class_<policy::NamedConcept, std::shared_ptr<policy::NamedConcept>>(m_policy, "NamedConcept")
        .def("__str__", py::overload_cast<>(&policy::NamedConcept::str, py::const_))
        .def("get_key", &policy::NamedConcept::get_key)
        .def("get_element", &policy::NamedConcept::get_element)
    ;

    py::class_<policy::NamedRole, std::shared_ptr<policy::NamedRole>>(m_policy, "NamedRole")
        .def("__str__", py::overload_cast<>(&policy::NamedRole::str, py::const_))
        .def("get_key", &policy::NamedRole::get_key)
        .def("get_element", &policy::NamedRole::get_element)
    ;

    py::class_<policy::BaseCondition, std::shared_ptr<policy::BaseCondition>>(m_policy, "BaseCondition")
        .def("__str__", py::overload_cast<>(&policy::BaseCondition::str, py::const_))
        .def("evaluate", py::overload_cast<const core::State&>(&policy::BaseCondition::evaluate, py::const_))
        .def("evaluate",  py::overload_cast<const core::State&, core::DenotationsCaches&>(&policy::BaseCondition::evaluate, py::const_))
        .def("get_index", &policy::BaseCondition::get_index)
    ;

    py::class_<policy::PositiveBooleanCondition, policy::BaseCondition, std::shared_ptr<policy::PositiveBooleanCondition>>(m_policy, "PositiveBooleanCondition")
        .def("get_named_element", &policy::PositiveBooleanCondition::get_named_element)
    ;

    py::class_<policy::NegativeBooleanCondition, policy::BaseCondition, std::shared_ptr<policy::NegativeBooleanCondition>>(m_policy, "NegativeBooleanCondition")
        .def("get_named_element", &policy::NegativeBooleanCondition::get_named_element)
    ;

    py::class_<policy::GreaterNumericalCondition, policy::BaseCondition, std::shared_ptr<policy::GreaterNumericalCondition>>(m_policy, "GreaterNumericalCondition")
        .def("get_named_element", &policy::GreaterNumericalCondition::get_named_element)
    ;

    py::class_<policy::EqualNumericalCondition, policy::BaseCondition, std::shared_ptr<policy::EqualNumericalCondition>>(m_policy, "EqualNumericalCondition")
        .def("get_named_element", &policy::EqualNumericalCondition::get_named_element)
    ;

    py::class_<policy::BaseEffect, std::shared_ptr<policy::BaseEffect>>(m_policy, "BaseEffect")
        .def("__str__", py::overload_cast<>(&policy::BaseEffect::str, py::const_))
        .def("evaluate", py::overload_cast<const core::State&, const core::State&>(&policy::BaseEffect::evaluate, py::const_))
        .def("evaluate",  py::overload_cast<const core::State&, const core::State&, core::DenotationsCaches&>(&policy::BaseEffect::evaluate, py::const_))
        .def("get_index", &policy::BaseEffect::get_index)
    ;

    py::class_<policy::PositiveBooleanEffect, policy::BaseEffect, std::shared_ptr<policy::PositiveBooleanEffect>>(m_policy, "PositiveBooleanEffect")
        .def("get_named_element", &policy::PositiveBooleanEffect::get_named_element)
    ;

    py::class_<policy::NegativeBooleanEffect, policy::BaseEffect, std::shared_ptr<policy::NegativeBooleanEffect>>(m_policy, "NegativeBooleanEffect")
        .def("get_named_element", &policy::NegativeBooleanEffect::get_named_element)
    ;

    py::class_<policy::UnchangedBooleanEffect, policy::BaseEffect, std::shared_ptr<policy::UnchangedBooleanEffect>>(m_policy, "UnchangedBooleanEffect")
        .def("get_named_element", &policy::UnchangedBooleanEffect::get_named_element)
    ;

    py::class_<policy::DecrementNumericalEffect, policy::BaseEffect, std::shared_ptr<policy::DecrementNumericalEffect>>(m_policy, "DecrementNumericalEffect")
        .def("get_named_element", &policy::DecrementNumericalEffect::get_named_element)
    ;

    py::class_<policy::DecrementOrUnchangedNumericalEffect, policy::BaseEffect, std::shared_ptr<policy::DecrementOrUnchangedNumericalEffect>>(m_policy, "DecrementOrUnchangedNumericalEffect")
        .def("get_named_element", &policy::DecrementOrUnchangedNumericalEffect::get_named_element)
    ;

    py::class_<policy::IncrementNumericalEffect, policy::BaseEffect, std::shared_ptr<policy::IncrementNumericalEffect>>(m_policy, "IncrementNumericalEffect")
        .def("get_named_element", &policy::IncrementNumericalEffect::get_named_element)
    ;

    py::class_<policy::IncrementOrUnchangedNumericalEffect, policy::BaseEffect, std::shared_ptr<policy::IncrementOrUnchangedNumericalEffect>>(m_policy, "IncrementOrUnchangedNumericalEffect")
        .def("get_named_element", &policy::IncrementOrUnchangedNumericalEffect::get_named_element)
    ;

    py::class_<policy::UnchangedNumericalEffect, policy::BaseEffect, std::shared_ptr<policy::UnchangedNumericalEffect>>(m_policy, "UnchangedNumericalEffect")
        .def("get_named_element", &policy::UnchangedNumericalEffect::get_named_element)
    ;

    py::class_<policy::Rule, std::shared_ptr<policy::Rule>>(m_policy, "Rule")
        .def("__str__", py::overload_cast<>(&policy::Rule::str, py::const_))
        .def("evaluate_conditions", py::overload_cast<const core::State&>(&policy::Rule::evaluate_conditions, py::const_))
        .def("evaluate_conditions", py::overload_cast<const core::State&, core::DenotationsCaches&>(&policy::Rule::evaluate_conditions, py::const_))
        .def("evaluate_effects", py::overload_cast<const core::State&, const core::State&>(&policy::Rule::evaluate_effects, py::const_))
        .def("evaluate_effects", py::overload_cast<const core::State&, const core::State&, core::DenotationsCaches&>(&policy::Rule::evaluate_effects, py::const_))
        .def("get_index", &policy::Rule::get_index)
        .def("get_conditions", &policy::Rule::get_conditions)
        .def("get_effects", &policy::Rule::get_effects)
    ;

    py::class_<policy::Policy, std::shared_ptr<policy::Policy>>(m_policy, "Policy")
        .def("__str__", py::overload_cast<>(&policy::Policy::str, py::const_))
        .def("evaluate", py::overload_cast<const core::State&, const core::State&>(&policy::Policy::evaluate, py::const_))
        .def("evaluate", py::overload_cast<const core::State&, const core::State&, core::DenotationsCaches&>(&policy::Policy::evaluate, py::const_))
        .def("evaluate_conditions", py::overload_cast<const core::State&>(&policy::Policy::evaluate_conditions, py::const_))
        .def("evaluate_conditions", py::overload_cast<const core::State&, core::DenotationsCaches&>(&policy::Policy::evaluate_conditions, py::const_))
        .def("evaluate_effects", py::overload_cast<const core::State&, const core::State&, const std::vector<std::shared_ptr<const policy::Rule>>&>(&policy::Policy::evaluate_effects, py::const_))
        .def("evaluate_effects", py::overload_cast<const core::State&, const core::State&, const std::vector<std::shared_ptr<const policy::Rule>>&, core::DenotationsCaches&>(&policy::Policy::evaluate_effects, py::const_))
        .def("get_rules", &policy::Policy::get_rules)
        .def("get_booleans", &policy::Policy::get_booleans)
        .def("get_numericals", &policy::Policy::get_numericals)
        .def("get_concepts", &policy::Policy::get_concepts)
    ;

// py::overload_cast<>(&policy::PolicyFactory::str, py::const_)

    py::class_<policy::PolicyFactory, std::shared_ptr<policy::PolicyFactory>>(m_policy, "PolicyFactory")
        .def(py::init<std::shared_ptr<core::SyntacticElementFactory>>())
        .def("make_boolean", &policy::PolicyFactory::make_boolean)
        .def("make_numerical", &policy::PolicyFactory::make_numerical)
        .def("make_concept", &policy::PolicyFactory::make_concept)
        .def("make_role", &policy::PolicyFactory::make_role)

        .def("make_pos_condition", &policy::PolicyFactory::make_pos_condition)
        .def("make_neg_condition", &policy::PolicyFactory::make_neg_condition)

        .def("make_gt_condition", py::overload_cast<const std::shared_ptr<const policy::NamedNumerical>&>(&policy::PolicyFactory::make_gt_condition))
        .def("make_eq_condition", py::overload_cast<const std::shared_ptr<const policy::NamedNumerical>&>(&policy::PolicyFactory::make_eq_condition))

        .def("make_gt_condition", py::overload_cast<const std::shared_ptr<const policy::NamedConcept>&>(&policy::PolicyFactory::make_gt_condition))
        .def("make_eq_condition", py::overload_cast<const std::shared_ptr<const policy::NamedConcept>&>(&policy::PolicyFactory::make_eq_condition))

        .def("make_pos_effect", &policy::PolicyFactory::make_pos_effect)
        .def("make_neg_effect", &policy::PolicyFactory::make_neg_effect)
        .def("make_bot_effect", py::overload_cast<const std::shared_ptr<const policy::NamedBoolean>&>(&policy::PolicyFactory::make_bot_effect))

        .def("make_inc_effect", py::overload_cast<const std::shared_ptr<const policy::NamedNumerical>&>(&policy::PolicyFactory::make_inc_effect))
        .def("make_inc_bot_effect", py::overload_cast<const std::shared_ptr<const policy::NamedNumerical>&>(&policy::PolicyFactory::make_inc_bot_effect))
        .def("make_dec_effect", py::overload_cast<const std::shared_ptr<const policy::NamedNumerical>&>(&policy::PolicyFactory::make_dec_effect))
        .def("make_dec_bot_effect", py::overload_cast<const std::shared_ptr<const policy::NamedNumerical>&>(&policy::PolicyFactory::make_dec_bot_effect))
        .def("make_bot_effect", py::overload_cast<const std::shared_ptr<const policy::NamedNumerical>&>(&policy::PolicyFactory::make_bot_effect))
        .def("make_gt_effect", py::overload_cast<const std::shared_ptr<const policy::NamedNumerical>&>(&policy::PolicyFactory::make_gt_effect))
        .def("make_eq_effect", py::overload_cast<const std::shared_ptr<const policy::NamedNumerical>&>(&policy::PolicyFactory::make_eq_effect))

        .def("make_inc_effect", py::overload_cast<const std::shared_ptr<const policy::NamedConcept>&>(&policy::PolicyFactory::make_inc_effect))
        .def("make_dec_effect", py::overload_cast<const std::shared_ptr<const policy::NamedConcept>&>(&policy::PolicyFactory::make_dec_effect))
        .def("make_gt_effect", py::overload_cast<const std::shared_ptr<const policy::NamedConcept>&>(&policy::PolicyFactory::make_gt_effect))
        .def("make_eq_effect", py::overload_cast<const std::shared_ptr<const policy::NamedConcept>&>(&policy::PolicyFactory::make_eq_effect))
        .def("make_bot_effect", py::overload_cast<const std::shared_ptr<const policy::NamedConcept>&>(&policy::PolicyFactory::make_bot_effect))

        .def("make_rule", &policy::PolicyFactory::make_rule)

        .def("make_policy", &policy::PolicyFactory::make_policy)
        .def("parse_policy", py::overload_cast<const std::string&, const std::string&>(&policy::PolicyFactory::parse_policy), py::arg("description"), py::arg("filename") = "")
    ;

    py::class_<policy::PolicyMinimizer>(m_policy, "PolicyMinimizer")
        .def(py::init<>())
        .def("minimize", py::overload_cast<const std::shared_ptr<const policy::Policy>&, policy::PolicyFactory&>(&policy::PolicyMinimizer::minimize, py::const_))
        .def("minimize", py::overload_cast<const std::shared_ptr<const policy::Policy>&, const policy::StatePairs&, const policy::StatePairs&, policy::PolicyFactory&>(&policy::PolicyMinimizer::minimize, py::const_))
    ;
}