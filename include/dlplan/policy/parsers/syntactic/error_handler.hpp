#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SYNTACTIC_ERROR_HANDLER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SYNTACTIC_ERROR_HANDLER_HPP_

#include <map>
#include <iostream>

#include "include/dlplan/core/parsers/syntactic/error_handler.hpp"


namespace dlplan::policy
{
    namespace x3 = boost::spirit::x3;

    struct error_handler_policy : dlplan::core::error_handler_core {
        error_handler_policy() : dlplan::core::error_handler_core() {
            id_map["feature_condition"] = "BaseCondition";
            id_map["feature_effect"] = "BaseEffect";

        }
    };
}

#endif
