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

    // ErrorMessages must be default constructible
    template<typename ErrorMessages>
    struct error_handler_base
    {
        error_handler_base() { }

        template <typename Iterator, typename Exception, typename Context>
        x3::error_handler_result on_error(
            Iterator& /*first*/, Iterator const& /*last*/
          , Exception const& x, Context const& context) {
            {
                auto& error_counter = x3::get<dlplan::parsers::error_counter_tag>(context).get();
                if (error_counter.count > 0) {
                    // We only print the first occurence of an error
                    return x3::error_handler_result::fail;
                }
                error_counter.increment();

                // Construct a nice error message using the map.
                std::string which = x.which();
                const auto& id_map = error_messages.id_map;
                auto iter = id_map.find(which);
                if (iter != id_map.end())
                    which = iter->second;

                std::string message = "Error! Expecting: " + which + " here:";
                auto& error_handler = x3::get<dlplan::parsers::error_handler_tag>(context).get();
                error_handler(x.where(), message);

                return x3::error_handler_result::fail;
            }
        }

        ErrorMessages error_messages;
    };



}

#endif
