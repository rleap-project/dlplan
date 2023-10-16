#ifndef DLPLAN_SRC_CORE_PARSERS_ELEMENTS_STAGE_1_ERROR_MESSAGES_HPP_
#define DLPLAN_SRC_CORE_PARSERS_ELEMENTS_STAGE_1_ERROR_MESSAGES_HPP_

#include <map>
#include <iostream>


namespace dlplan::core::parsers::elements
{
    struct error_messages
    {
        error_messages() = default;

        static std::map<std::string, std::string> id_map;
    };
}

#endif
