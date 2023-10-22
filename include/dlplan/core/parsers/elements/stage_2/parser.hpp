#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_ELEMENTS_STAGE_2_PARSER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_ELEMENTS_STAGE_2_PARSER_HPP_

#include <tuple>
#include <variant>

#include "include/dlplan/core.h"

#include "include/dlplan/common/parsers/config.hpp"
#include "include/dlplan/core/parsers/elements/stage_1/ast.hpp"


namespace dlplan::core::parsers::elements::stage_2::parser {

extern std::string parse(const stage_1::ast::Name& node, const dlplan::common::parsers::error_handler_type&, SyntacticElementFactory&);

extern core::Constant parse(const stage_1::ast::Constant& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern core::Predicate parse(const stage_1::ast::Predicate& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern int parse(const stage_1::ast::Integer& node, const dlplan::common::parsers::error_handler_type&, SyntacticElementFactory&);

extern int parse(const stage_1::ast::Position& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Boolean> parse(const stage_1::ast::EmptyBoolean& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Boolean> parse(const stage_1::ast::InclusionBoolean& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Boolean> parse(const stage_1::ast::NullaryBoolean& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const stage_1::ast::AllConcept& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const stage_1::ast::AndConcept& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const stage_1::ast::BotConcept&, const dlplan::common::parsers::error_handler_type&, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const stage_1::ast::DiffConcept& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const stage_1::ast::EqualConcept& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const stage_1::ast::NotConcept& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const stage_1::ast::OneOfConcept& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const stage_1::ast::OrConcept& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const stage_1::ast::PrimitiveConcept& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const stage_1::ast::ProjectionConcept& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const stage_1::ast::SomeConcept& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const stage_1::ast::SubsetConcept& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const stage_1::ast::TopConcept&, const dlplan::common::parsers::error_handler_type&, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Numerical> parse(const stage_1::ast::ConceptDistanceNumerical& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Numerical> parse(const stage_1::ast::CountNumerical& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Numerical> parse(const stage_1::ast::RoleDistanceNumerical& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Numerical> parse(const stage_1::ast::SumConceptDistanceNumerical& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Numerical> parse(const stage_1::ast::SumRoleDistanceNumerical& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const stage_1::ast::AndRole& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const stage_1::ast::ComposeRole& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const stage_1::ast::DiffRole& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const stage_1::ast::IdentityRole& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const stage_1::ast::InverseRole& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const stage_1::ast::NotRole& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const stage_1::ast::OrRole& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const stage_1::ast::PrimitiveRole& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const stage_1::ast::RestrictRole& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const stage_1::ast::TopRole&, const dlplan::common::parsers::error_handler_type&, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const stage_1::ast::TransitiveClosureRole& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const stage_1::ast::TransitiveReflexiveClosureRole& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern boost::variant<std::shared_ptr<const core::Concept>, std::shared_ptr<const core::Role>> parse(const stage_1::ast::ConceptOrRole& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const Boolean> parse(const stage_1::ast::Boolean& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const Numerical> parse(const stage_1::ast::Numerical& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const Concept> parse(const stage_1::ast::Concept& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const Role> parse(const stage_1::ast::Role& node, const dlplan::common::parsers::error_handler_type& error_handler, SyntacticElementFactory& context);

}

#endif

