#include "parser_def.hpp"

#include "src/core/parsers/common/config.hpp"


namespace dlplan::core::parsers::elements::stage_1::parser
{
    BOOST_SPIRIT_INSTANTIATE(
        element_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        boolean_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        numerical_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        concept_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        role_type, iterator_type, context_type)
}
