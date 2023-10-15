#ifndef SRC_CORE_PARSERS_COMMON_CONFIG_HPP_
#define SRC_CORE_PARSERS_COMMON_CONFIG_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>


namespace dlplan::core::parsers
{
    namespace x3 = boost::spirit::x3;

    // Our Iterator Type
    typedef std::string::const_iterator iterator_type;

    // The Phrase Parse Context
    typedef
        x3::phrase_parse_context<x3::ascii::space_type>::type
    phrase_context_type;

    // X3 Error Handler Utility
    template <typename Iterator>
    using error_handler = x3::error_handler<Iterator>;

    // tag used to get our error handler from the context
    using error_handler_tag = x3::error_handler_tag;

    // Our Error Handler
    typedef error_handler<iterator_type> error_handler_type;

    // Combined Error Handler and Phrase Parse Context
    typedef x3::context<
        error_handler_tag
      , std::reference_wrapper<error_handler_type>
      , phrase_context_type>
    context_type;
}

#endif