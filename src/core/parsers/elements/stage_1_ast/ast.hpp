#ifndef SRC_CORE_PARSERS_ELEMENTS_STAGE_1_AST_HPP_
#define SRC_CORE_PARSERS_ELEMENTS_STAGE_1_AST_HPP_

#include <vector>

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>


namespace dlplan::core::parsers::elements::stage_1::ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  The AST
    ///////////////////////////////////////////////////////////////////////////
    namespace x3 = boost::spirit::x3;

    struct Name;

    /* Basic character compounds */
    struct Name : x3::position_tagged {
        char alphabetical;
        std::string suffix;
    };


    struct Concept {

    };

    struct Role {

    };

    struct Boolean {

    };

    struct Numerical {

    };

    struct Element {

    };
}


#endif