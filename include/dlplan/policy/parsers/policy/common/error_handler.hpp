#ifndef DLPLAN_SRC_POLICY_PARSERS_POLICY_COMMON_ERROR_HANDLER_HPP_
#define DLPLAN_SRC_POLICY_PARSERS_POLICY_COMMON_ERROR_HANDLER_HPP_

#include <map>
#include <iostream>

#include "include/dlplan/common/parsers/config.hpp"


namespace dlplan::policy::parsers::policy
{
    namespace x3 = boost::spirit::x3;

    ////////////////////////////////////////////////////////////////////////////
    //  Our error handler
    ////////////////////////////////////////////////////////////////////////////

    struct error_handler_base
    {
        error_handler_base();

        template <typename Iterator, typename Exception, typename Context>
        x3::error_handler_result on_error(
            Iterator& first, Iterator const& last
          , Exception const& x, Context const& context);

        std::map<std::string, std::string> id_map;
    };

    //std::map<std::string, std::string> error_handler_base::id_map = {};

    ////////////////////////////////////////////////////////////////////////////
    // Implementation
    ////////////////////////////////////////////////////////////////////////////

    inline error_handler_base::error_handler_base()
    {
        // TODO: add more precise error messages.
        id_map["name"] = "name";
    }

    template <typename Iterator, typename Exception, typename Context>
    inline x3::error_handler_result
    error_handler_base::on_error(
        Iterator& /*first*/, Iterator const& /*last*/
      , Exception const& x, Context const& context)
    {
        auto& error_counter = x3::get<error_counter_tag>(context).get();
        if (error_counter.count > 0) {
            // We only print the first occurence of an error
            return x3::error_handler_result::fail;
        }
        error_counter.increment();

        // Construct a nice error message using the map.
        std::string which = x.which();
        auto iter = id_map.find(which);
        if (iter != id_map.end())
            which = iter->second;

        std::string message = "Error! Expecting: " + which + " here:";
        auto& error_handler = x3::get<error_handler_tag>(context).get();
        error_handler(x.where(), message);

        return x3::error_handler_result::fail;
    }
}

#endif
