#include "parser_def.hpp"

#include "include/dlplan/common/parsers/config.hpp"


namespace dlplan::core::parsers::elements::stage_1::parser
{
    BOOST_SPIRIT_INSTANTIATE(
        element_type, dlplan::parsers::iterator_type, dlplan::parsers::context_type)

    BOOST_SPIRIT_INSTANTIATE(
        boolean_type, dlplan::parsers::iterator_type, dlplan::parsers::context_type)

    BOOST_SPIRIT_INSTANTIATE(
        numerical_type, dlplan::parsers::iterator_type, dlplan::parsers::context_type)

    BOOST_SPIRIT_INSTANTIATE(
        concept_type, dlplan::parsers::iterator_type, dlplan::parsers::context_type)

    BOOST_SPIRIT_INSTANTIATE(
        role_type, dlplan::parsers::iterator_type, dlplan::parsers::context_type)
}
