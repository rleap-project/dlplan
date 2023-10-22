#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_ELEMENTS_STAGE_1_ERROR_HANDLER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_ELEMENTS_STAGE_1_ERROR_HANDLER_HPP_

#include <map>
#include <iostream>

#include "include/dlplan/common/parsers/error_handler.hpp"


namespace dlplan::core::parsers::elements::stage_1
{
    namespace x3 = boost::spirit::x3;

    struct error_handler_core : dlplan::common::parsers::error_handler_base {
        error_handler_core() : dlplan::common::parsers::error_handler_base() {
            id_map["name"] = "Name";
            id_map["integer"] = "Integer";
            id_map["constant"] = "Constant";
            id_map["predicate"] = "Predicate";
            id_map["position"] = "Position";
            id_map["boolean"] = "Boolean";
            id_map["numerical"] = "Numerical";
            id_map["concept"] = "Concept";
            id_map["role"] = "Role";
            id_map["element"] = "Element";
            id_map["concept_or_role"] = "Concept or Role";
        }
    };
}

#endif
