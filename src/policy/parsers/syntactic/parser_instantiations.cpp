#include "parser_def.hpp"

#include "../../../../include/dlplan/common/parsers/config.hpp"



namespace dlplan::policy::parser
{
    using iterator_type = dlplan::iterator_type;
    using context_type = dlplan::context_type;

    BOOST_SPIRIT_INSTANTIATE(
        name_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        boolean_definition_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        boolean_implementation_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        boolean_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        boolean_reference_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        booleans_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        numerical_definition_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        numerical_implementation_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        numerical_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        numerical_reference_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        numericals_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        concept_definition_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        concept_implementation_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        concept_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        concept_reference_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        concepts_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        role_definition_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        role_implementation_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        role_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        role_reference_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        roles_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        positive_boolean_condition_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        negative_boolean_condition_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        greater_numerical_condition_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        equal_numerical_condition_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        positive_boolean_effect_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        negative_boolean_effect_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        unchanged_boolean_effect_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        increment_numerical_effect_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        increment_or_unchanged_numerical_effect_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        decrement_numerical_effect_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        decrement_or_unchanged_numerical_effect_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        unchanged_numerical_effect_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        greater_numerical_effect_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        equal_numerical_effect_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        increment_concept_effect_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        decrement_concept_effect_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        unchanged_concept_effect_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        greater_concept_effect_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        equal_concept_effect_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        feature_condition_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        feature_effect_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        rule_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        rules_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        policy_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        policy_root_type, iterator_type, context_type)
}
