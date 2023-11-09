#include "../../../include/dlplan/common/parsers/utility.hpp"


namespace dlplan {

bool in_bounds(
    const error_handler_type& error_handler,
    const iterator_type& iter, const iterator_type& end) {
    if ((error_handler.get_position_cache().first() > iter && error_handler.get_position_cache().first() < end) ||
        (error_handler.get_position_cache().last() > iter && error_handler.get_position_cache().last() < end)) {
        return false;
    }
    return true;
}

}
