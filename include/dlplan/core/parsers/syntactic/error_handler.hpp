#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_SYNTACTIC_ERROR_HANDLER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_SYNTACTIC_ERROR_HANDLER_HPP_

#include "../../../common/parsers/error_handler.hpp"

#include <map>
#include <iostream>


namespace dlplan::core
{
    namespace x3 = boost::spirit::x3;

    struct error_handler_core : dlplan::error_handler_base {
        error_handler_core() : dlplan::error_handler_base() {
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
