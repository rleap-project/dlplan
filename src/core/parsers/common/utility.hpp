#ifndef SRC_CORE_PARSERS_COMMON_UTILITY_HPP_
#define SRC_CORE_PARSERS_COMMON_UTILITY_HPP_

#include <map>

#include "include/dlplan/core/parsers/common/config.hpp"


namespace dlplan::core::parsers {

extern bool in_bounds(
    const error_handler_type& error_handler,
    const iterator_type& iter, const iterator_type& end);

}

#endif
