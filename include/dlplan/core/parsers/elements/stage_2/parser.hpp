#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_ELEMENTS_STAGE_2_PARSER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_ELEMENTS_STAGE_2_PARSER_HPP_

#include <tuple>
#include <variant>

#include "include/dlplan/core.h"

#include "include/dlplan/core/parsers/common/config.hpp"
#include "include/dlplan/core/parsers/elements/stage_1/ast.hpp"


namespace dlplan::core::parsers::elements::stage_2::parser {

extern std::shared_ptr<const Boolean> parse_boolean(
    const stage_1::ast::Boolean& node,
    const error_handler_type& error_handler,
    SyntacticElementFactory& element_factory);

extern std::shared_ptr<const Numerical> parse_numerical(
    const stage_1::ast::Numerical& node,
    const error_handler_type& error_handler,
    SyntacticElementFactory& element_factory);

extern std::shared_ptr<const Concept> parse_concept(
    const stage_1::ast::Concept& node,
    const error_handler_type& error_handler,
    SyntacticElementFactory& element_factory);

extern std::shared_ptr<const Role> parse_role(
    const stage_1::ast::Role& node,
    const error_handler_type& error_handler,
    SyntacticElementFactory& element_factory);

}

#endif

