#ifndef SRC_CORE_PARSERS_SYNTACTIC_AST_ADAPTED_HPP_
#define SRC_CORE_PARSERS_SYNTACTIC_AST_ADAPTED_HPP_

#include "../../../../include/dlplan/core/parsers/syntactic/ast.hpp"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>


// We need to tell fusion about our ast nodes
// to make them a first-class fusion citizens
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::Name, alphabetical, suffix)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::Constant, name)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::Predicate, name)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::Integer, value)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::Position, integer)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::EmptyBoolean, element)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::InclusionBoolean, element_left, element_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::NullaryBoolean, predicate)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::AllConcept, role, concept_)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::AndConcept, concept_left, concept_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::BotConcept)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::DiffConcept, concept_left, concept_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::EqualConcept, role_left, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::NotConcept, concept_)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::OneOfConcept, constant)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::OrConcept, concept_left, concept_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::PrimitiveConcept, predicate, pos)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::ProjectionConcept, role, pos)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::SomeConcept, role, concept_)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::SubsetConcept, role_left, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::TopConcept)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::ConceptDistanceNumerical, concept_left, role, concept_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::CountNumerical, element)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::RoleDistanceNumerical, role_left, role, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::SumConceptDistanceNumerical, concept_left, role, concept_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::SumRoleDistanceNumerical, role_left, role, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::AndRole, role_left, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::ComposeRole, role_left, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::DiffRole, role_left, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::IdentityRole, concept_)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::InverseRole, role)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::NotRole, role)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::OrRole, role_left, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::PrimitiveRole, predicate, pos_1, pos_2)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::RestrictRole, role, concept_)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::TilCRole, role, concept_)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::TopRole)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::TransitiveClosureRole, role)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::ast::TransitiveReflexiveClosureRole, role)

#endif
