#ifndef SRC_CORE_PARSERS_ELEMENTS_STAGE_1_PARSER_DEF_HPP_
#define SRC_CORE_PARSERS_ELEMENTS_STAGE_1_PARSER_DEF_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "src/core/parsers/elements/common/error_handler.hpp"

#include "ast.hpp"
#include "ast_adapted.hpp"
#include "parser_api.hpp"


namespace dlplan::core::parsers::elements::stage_1::parser
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::lexeme;

    using ascii::alpha;
    using ascii::alnum;
    using ascii::char_;
    using ascii::string;

    ///////////////////////////////////////////////////////////////////////////
    // Rule IDs
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass;

    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    x3::rule<NameClass, ast::Name> const
        name = "name";

    element_type const element = "element";


    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    const auto name_def = alpha >> lexeme[*(alnum | char_('-') | char_('_'))];
    const auto element_def = "";
    BOOST_SPIRIT_DEFINE(name, element)

    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass : x3::annotate_on_success {};
    struct Element : x3::annotate_on_success, error_handler_base {};
}

namespace dlplan::core::parsers::elements::stage_1
{
    parser::element_type const& element()
    {
        return parser::element;
    }
}

#endif