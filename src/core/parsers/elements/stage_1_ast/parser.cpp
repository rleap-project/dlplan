#include "parser.hpp"

#include <cassert>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <fstream>

#include "src/core/parsers/common/config.hpp"
#include "src/core/parsers/common/utility.hpp"
#include "src/core/parsers/elements/common/error_handler.hpp"

#include "ast.hpp"
#include "parser_api.hpp"


namespace dlplan::core::parsers::elements::stage_1::parser {

ast::Element parse_ast(
    iterator_type& iter,
    iterator_type end,
    error_handler_type& error_handler) {
    assert(in_bounds(error_handler, iter, end));

    // Our AST
    dlplan::core::parsers::elements::stage_1::ast::Element ast;

    // Our parser
    using boost::spirit::x3::with;
    using dlplan::core::parsers::error_handler_tag;
    auto const parser =
        // we pass our error handler to the parser so we can access
        // it later on in our on_error and on_sucess handlers
        with<error_handler_tag>(std::ref(error_handler))
        [
            dlplan::core::parsers::elements::stage_1::element()
        ];

    // Go forth and parse!
    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, parser, space, ast) && iter == end;
    if (!success)
    {
        throw std::runtime_error("Unsuccessful parse.");
    }

    return ast;
}

}

