#ifndef SRC_CORE_PARSERS_ELEMENTS_STAGE_1_PARSER_HPP_
#define SRC_CORE_PARSERS_ELEMENTS_STAGE_1_PARSER_HPP_

#include "ast.hpp"

#include "src/core/parsers/common/config.hpp"


namespace dlplan::core::parsers::elements::stage_1::parser {


/// @brief Parses the content from iter to end
extern ast::ElementWrapper parse_ast(
    iterator_type& iter,
    iterator_type end,
    error_handler_type& error_handler);

}

#endif
