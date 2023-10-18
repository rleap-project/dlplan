#ifndef DLPLAN_INCLUDE_DLPLAN_COMMON_PARSERS_CONFIG_HPP_
#define DLPLAN_INCLUDE_DLPLAN_COMMON_PARSERS_CONFIG_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>


namespace dlplan::common::parsers
{
    namespace x3 = boost::spirit::x3;

    // Our Iterator Type
    typedef std::string::const_iterator iterator_type;


    /* X3 Error Handler Utility */
    template <typename Iterator>
    using error_handler = x3::error_handler<Iterator>;

    using error_handler_tag = x3::error_handler_tag;

    typedef error_handler<iterator_type> error_handler_type;


    /* In case we need to provide context in the future. */
    struct parsing_context_tag;

    struct parsing_context_type {
        bool error_reported = false;
    };


    /* The Phrase Parse Context */
    typedef
        x3::phrase_parse_context<x3::ascii::space_type>::type
    phrase_context_type;


    /* Combined Error Handler and Phrase Parse Context */
    typedef x3::context<
            error_handler_tag,
            std::reference_wrapper<error_handler_type>,
            x3::context<
                parsing_context_tag,
                std::reference_wrapper<parsing_context_type>,
                phrase_context_type>>
    context_type;

}

#endif
