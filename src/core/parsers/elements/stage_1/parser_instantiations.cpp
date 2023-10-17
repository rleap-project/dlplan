#include "parser_def.hpp"

#include "include/dlplan/common/parsers/config.hpp"


namespace dlplan::core::parsers::elements::stage_1::parser
{
    using iterator_type = dlplan::common::parsers::iterator_type;
    using context_type = dlplan::common::parsers::context_type;

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
