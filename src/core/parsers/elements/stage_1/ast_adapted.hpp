#ifndef SRC_CORE_PARSERS_ELEMENTS_STAGE_1_AST_ADAPTED_HPP_
#define SRC_CORE_PARSERS_ELEMENTS_STAGE_1_AST_ADAPTED_HPP_

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include "include/dlplan/core/parsers/elements/stage_1/ast.hpp"


// We need to tell fusion about our ast nodes
// to make them a first-class fusion citizens
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::Name, alphabetical, suffix)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::Constant, name)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::Predicate, name)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::Position, value)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::Boolean, boolean)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::Concept, concept)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::Numerical, numerical)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::Role, role)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::Element, element)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::ConceptOrRole, inner)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::EmptyBoolean, element)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::InclusionBoolean, element_left, element_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::NullaryBoolean, predicate)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::AllConcept, role, concept_)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::AndConcept, concept_left, concept_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::BotConcept)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::DiffConcept, concept_left, concept_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::EqualConcept, role_left, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::NotConcept, concept_)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::OneOfConcept, constant)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::OrConcept, concept_left, concept_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::PrimitiveConcept, predicate, pos)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::ProjectionConcept, role, pos)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::SomeConcept, role, concept_)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::SubsetConcept, role_left, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::TopConcept)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::ConceptDistanceNumerical, concept_left, role, concept_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::CountNumerical, element)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::RoleDistanceNumerical, role_left, role, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::SumConceptDistanceNumerical, concept_left, role, concept_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::SumRoleDistanceNumerical, role_left, role, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::AndRole, role_left, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::ComposeRole, role_left, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::DiffRole, role_left, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::IdentityRole, concept_)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::InverseRole, role)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::NotRole, role)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::OrRole, role_left, role_right)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::PrimitiveRole, predicate, pos_1, pos_2)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::RestrictRole, role, concept_)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::TopRole)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::TransitiveClosureRole, role)
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::TransitiveReflexiveClosureRole, role)

#endif
