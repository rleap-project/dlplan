#include "parser_def.hpp"

#include "src/core/parsers/common/config.hpp"


namespace dlplan::core::parsers::elements::stage_1::parser
{
    BOOST_SPIRIT_INSTANTIATE(
        element_type, iterator_type, context_type)
}
