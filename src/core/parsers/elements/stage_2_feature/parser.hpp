#ifndef SRC_CORE_PARSERS_ELEMENTS_STAGE_2_PARSER_HPP_
#define SRC_CORE_PARSERS_ELEMENTS_STAGE_2_PARSER_HPP_

#include <tuple>
#include <variant>

#include "include/dlplan/core.h"

#include "src/core/parsers/common/config.hpp"
#include "src/core/parsers/elements/stage_1_ast/ast.hpp"

#include "context.hpp"


namespace dlplan::core::parsers::elements::stage_2::parser {

extern std::shared_ptr<const Boolean> parse_boolean(
    Context& context,
    const error_handler_type& error_handler,
    const stage_1::ast::Element& node);

extern std::shared_ptr<const Numerical> parse_numerical(
    Context& context,
    const error_handler_type& error_handler,
    const stage_1::ast::Element& node);

extern std::shared_ptr<const Concept> parse_concept(
    Context& context,
    const error_handler_type& error_handler,
    const stage_1::ast::Element& node);

extern std::shared_ptr<const Role> parse_role(
    Context& context,
    const error_handler_type& error_handler,
    const stage_1::ast::Element& node);

}

#endif

