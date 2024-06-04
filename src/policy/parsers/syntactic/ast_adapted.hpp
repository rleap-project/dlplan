#ifndef DLPLAN_SRC_POLICY_PARSERS_SYNTACTIC_AST_ADAPTED_HPP_
#define DLPLAN_SRC_POLICY_PARSERS_SYNTACTIC_AST_ADAPTED_HPP_

#include "../../../../include/dlplan/policy/parsers/syntactic/ast.hpp"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>


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
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::ConceptImplementation, concept_)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Concept, definition, implementation)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::ConceptReference, key)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Concepts, definitions)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::RoleDefinition, key)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::RoleImplementation, role)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Role, definition, implementation)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::RoleReference, key)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Roles, definitions)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::PositiveBooleanCondition, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::NegativeBooleanCondition, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::GreaterNumericalCondition, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::EqualNumericalCondition, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::GreaterConceptCondition, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::EqualConceptCondition, reference)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::PositiveBooleanEffect, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::NegativeBooleanEffect, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::UnchangedBooleanEffect, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::IncrementNumericalEffect, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::IncrementOrUnchangedNumericalEffect, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::DecrementNumericalEffect, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::DecrementOrUnchangedNumericalEffect, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::UnchangedNumericalEffect, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::GreaterNumericalEffect, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::EqualNumericalEffect, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::IncrementConceptEffect, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::DecrementConceptEffect, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::UnchangedConceptEffect, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::GreaterConceptEffect, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::EqualConceptEffect, reference)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Rule, feature_conditions, feature_effects)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Rules, rules)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::ast::Policy, booleans, numericals, concepts, roles, rules)

#endif
