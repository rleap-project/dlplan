#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_SEMANTIC_PARSER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_SEMANTIC_PARSER_HPP_

#include "../syntactic/ast.hpp"

#include "../../../common/parsers/config.hpp"
#include "../../../core.h"

#include <tuple>
#include <variant>


namespace dlplan::core {

extern std::string parse(const ast::Name& node, const dlplan::error_handler_type&, SyntacticElementFactory&);

extern core::Constant parse(const ast::Constant& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern core::Predicate parse(const ast::Predicate& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern int parse(const ast::Integer& node, const dlplan::error_handler_type&, SyntacticElementFactory&);

extern int parse(const ast::Position& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Boolean> parse(const ast::EmptyBoolean& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Boolean> parse(const ast::InclusionBoolean& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Boolean> parse(const ast::NullaryBoolean& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const ast::AllConcept& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const ast::AndConcept& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const ast::BotConcept&, const dlplan::error_handler_type&, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const ast::DiffConcept& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const ast::EqualConcept& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const ast::NotConcept& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const ast::OneOfConcept& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const ast::OrConcept& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const ast::PrimitiveConcept& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const ast::ProjectionConcept& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const ast::SomeConcept& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const ast::SubsetConcept& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Concept> parse(const ast::TopConcept&, const dlplan::error_handler_type&, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Numerical> parse(const ast::ConceptDistanceNumerical& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Numerical> parse(const ast::CountNumerical& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Numerical> parse(const ast::RoleDistanceNumerical& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Numerical> parse(const ast::SumConceptDistanceNumerical& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Numerical> parse(const ast::SumRoleDistanceNumerical& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const ast::AndRole& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const ast::ComposeRole& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const ast::DiffRole& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const ast::IdentityRole& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const ast::InverseRole& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const ast::NotRole& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const ast::OrRole& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const ast::PrimitiveRole& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const ast::RestrictRole& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const ast::TilCRole& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const ast::TopRole&, const dlplan::error_handler_type&, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const ast::TransitiveClosureRole& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const core::Role> parse(const ast::TransitiveReflexiveClosureRole& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern boost::variant<std::shared_ptr<const core::Concept>, std::shared_ptr<const core::Role>> parse(const ast::ConceptOrRole& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const Boolean> parse(const ast::Boolean& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const Numerical> parse(const ast::Numerical& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const Concept> parse(const ast::Concept& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

extern std::shared_ptr<const Role> parse(const ast::Role& node, const dlplan::error_handler_type& error_handler, SyntacticElementFactory& context);

}

#endif

