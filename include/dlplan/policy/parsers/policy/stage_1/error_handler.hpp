#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_1_ERROR_HANDLER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_1_ERROR_HANDLER_HPP_

#include <map>
#include <iostream>

#include "include/dlplan/core/parsers/elements/stage_1/error_handler.hpp"


namespace dlplan::policy::parsers::policy::stage_1
{
    namespace x3 = boost::spirit::x3;

    struct error_handler_policy : dlplan::core::parsers::elements::stage_1::error_handler_core {
        error_handler_policy() : dlplan::core::parsers::elements::stage_1::error_handler_core() {
            // TODO: add more precise error messages.
            id_map["name_inner"] = "Name";
            id_map["feature_condition_entry_inner"] = "BaseCondition";
            id_map["feature_effect_entry_inner"] = "BaseEffect";

        }
    };
}

#endif
