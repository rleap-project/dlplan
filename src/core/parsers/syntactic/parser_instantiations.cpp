#include "parser_def.hpp"

#include "../../../../include/dlplan/common/parsers/config.hpp"


namespace dlplan::core::parser
{
    using iterator_type = dlplan::iterator_type;
    using phrase_context_type = dlplan::phrase_context_type;
    using context_type = dlplan::context_type;

    BOOST_SPIRIT_INSTANTIATE(
        element_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        element_root_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        boolean_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        boolean_root_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        numerical_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        numerical_root_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        concept_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        concept_root_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        role_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        role_root_type, iterator_type, context_type)
}
