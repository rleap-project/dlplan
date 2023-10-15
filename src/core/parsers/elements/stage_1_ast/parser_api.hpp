#ifndef SRC_CORE_PARSERS_ELEMENTS_STAGE_1_PARSER_API_HPP_
#define SRC_CORE_PARSERS_ELEMENTS_STAGE_1_PARSER_API_HPP_

#include <boost/spirit/home/x3.hpp>

#include "ast.hpp"


namespace dlplan::core::parsers::elements::stage_1
{
    namespace x3 = boost::spirit::x3;

    ///////////////////////////////////////////////////////////////////////////
    // rexpr public interface
    ///////////////////////////////////////////////////////////////////////////
    namespace parser {
        struct ElementClass;
        typedef x3::rule<ElementClass, ast::Element> element_type;
        BOOST_SPIRIT_DECLARE(element_type)
    }
    parser::element_type const& element();
}

#endif