#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_ELEMENTS_COMMON_ERROR_HANDLER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_ELEMENTS_COMMON_ERROR_HANDLER_HPP_

#include <map>
#include <iostream>

#include "include/dlplan/common/parsers/config.hpp"


namespace dlplan::common::parsers
{
    namespace x3 = boost::spirit::x3;

    ////////////////////////////////////////////////////////////////////////////
    //  Our error handler
    ////////////////////////////////////////////////////////////////////////////

    struct error_handler_base
    {
        std::string error_message;

        // Spirit calls the default constructor
        error_handler_base() : error_message("") { }

        error_handler_base(const std::string& message) : error_message(message) { }

        virtual ~error_handler_base() { }

        template <typename Iterator, typename Exception, typename Context>
        x3::error_handler_result on_error(
            Iterator& /*first*/, Iterator const& /*last*/
          , Exception const& x, Context const& context) {
            {
                auto& parsing_context = x3::get<parsing_context_tag>(context).get();
                if (parsing_context.error_reported) {
                    // We only print the first occurence of an error
                    return x3::error_handler_result::fail;
                }
                parsing_context.error_reported = true;

                std::string which = x.which();
                // Use our message if defined
                if (error_message != "") {
                    which = error_message;
                }

                std::string message = "Error! Expecting: " + which + " here:";
                auto& error_handler = x3::get<error_handler_tag>(context).get();
                error_handler(x.where(), message);

                return x3::error_handler_result::fail;
            }
        }
    };




}

#endif
