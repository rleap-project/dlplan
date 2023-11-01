#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_COMMON_PARSERS_ERROR_HANDLER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_CORE_COMMON_PARSERS_ERROR_HANDLER_HPP_

#include "config.hpp"

#include <unordered_map>
#include <iostream>


namespace dlplan
{
    namespace x3 = boost::spirit::x3;

    ////////////////////////////////////////////////////////////////////////////
    // Our error handler
    ////////////////////////////////////////////////////////////////////////////

    struct error_handler_base
    {
        std::unordered_map<std::string, std::string> id_map;

        error_handler_base() { }

        template <typename Iterator, typename Exception, typename Context>
        x3::error_handler_result on_error(
            Iterator& /*first*/, Iterator const& /*last*/
          , Exception const& x, Context const& context) {
            {
                std::string which = x.which();
                auto iter = id_map.find(which);
                if (iter != id_map.end()) {
                    which = iter->second;
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
