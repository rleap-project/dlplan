#ifndef DLPLAN_SRC_POLICY_PARSERS_SYNTACTIC_AST_ADAPTED_HPP_
#define DLPLAN_SRC_POLICY_PARSERS_SYNTACTIC_AST_ADAPTED_HPP_

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include "include/dlplan/policy/parsers/syntactic/ast.hpp"


// We need to tell fusion about our ast nodes
// to make them a first-class fusion citizens
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Name, alphabetical, suffix)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::BooleanDefinition, key)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::BooleanImplementation, boolean)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Boolean, definition, implementation)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::BooleanReference, key)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Booleans, definitions)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::NumericalDefinition, key)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::NumericalImplementation, numerical)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Numerical, definition, implementation)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::NumericalReference, key)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Numericals, definitions)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::ConceptDefinition, key)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::ConceptImplementation, concept)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Concept, definition, implementation)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::ConceptReference, key)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Concepts, definitions)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::RoleDefinition, key)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::RoleImplementation, role)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Role, definition, implementation)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::RoleReference, key)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Roles, definitions)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::PositiveBooleanConditionEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::NegativeBooleanConditionEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::GreaterNumericalConditionEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::EqualNumericalConditionEntry, reference)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::PositiveBooleanEffectEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::NegativeBooleanEffectEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::UnchangedBooleanEffectEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::IncrementNumericalEffectEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::DecrementNumericalEffect, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::UnchangedNumericalEffect, reference)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Rule, feature_conditions, feature_effects)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Rules, rules)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Policy, booleans, numericals, rules)

#endif
