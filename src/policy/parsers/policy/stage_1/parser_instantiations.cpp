#include "parser_def.hpp"

#include "include/dlplan/common/parsers/config.hpp"



namespace dlplan::policy::parsers::policy::stage_1::parser
{
    using iterator_type = dlplan::common::parsers::iterator_type;
    using context_type = dlplan::common::parsers::context_type;

    BOOST_SPIRIT_INSTANTIATE(
        name_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        boolean_definition_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        boolean_reference_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        booleans_entry_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        numerical_definition_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        numerical_reference_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        numericals_entry_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        positive_boolean_condition_entry_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        negative_boolean_condition_entry_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        greater_numerical_condition_entry_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        equal_numerical_condition_entry_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        positive_boolean_effect_entry_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        negative_boolean_effect_entry_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        unchanged_boolean_effect_entry_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        increment_numerical_effect_entry_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        decrement_numerical_effect_entry_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        unchanged_numerical_effect_entry_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        feature_condition_entry_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        feature_effect_entry_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        rule_entry_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        rules_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        policy_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        policy_root_type, iterator_type, context_type)
}
